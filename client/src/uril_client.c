/*
 * Copyright (c) 2016, Intel Corporation, Intel Mobile Communication
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "uril_client_private.h"
#include "uril_client_internal.h"
#include "uril_cli_log.h"
#include "uril_protocol.h"
#include "uril_client.h"

#include "service_connector.h"
#include "service_connector/unix_socket_service_connector.h"

#include "osal/osal_mutex.h"
#include "osal/osal_condition.h"
#include "osal/osal_mutex_unix.h"
#include "osal/osal_condition_unix.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(*a))
#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

static volatile uril_client_handle_t s_unique_handle = 0;
static volatile uint32_t s_unique_msg_id = 0;
static osal_mutex_t s_init_mutex = UNIX_MUTEX;
static osal_mutex_t s_unique_id_gen_mutex = UNIX_MUTEX;
osal_mutex_t cmd_mutex = UNIX_MUTEX;

uril_cli_ctx_t context;
service_connector_t* service_connector = NULL;

void uril_client_set_service_connector(service_connector_t* connector) {
    URIL_CLI_TRACE_D("set_service_connector: %s",
            connector != NULL ? connector->get_name() : "NULL");
    service_connector = connector;
}

struct async_params {
    int success;
    osal_condition_t* signal_cond;
    osal_mutex_t* mutex;
    int condition;
    void* resp;
    size_t resp_len;
};

inline static void msg_print_no_check(const uril_protocol_msg_t* msg) {
    URIL_CLI_TRACE_D("msg { %u, %x, %u, %u }", msg->sim_id, msg->msg_id,
            msg->unique_id, msg->len);
}

static uril_cli_pending_op_t* acquire_free_op_slot() {
    uril_cli_pending_op_t* ret = NULL;
    size_t i = 0;

    for (i = 0; i < ARRAY_SIZE(context.pending_ops) && ret == NULL; i++) {
        if (!context.pending_ops[i].busy) {
            ret = &(context.pending_ops[i]);
            ret->busy = 1;
        }
    }
    return ret;
}

static uril_cli_pending_op_t* lookup_pending_op(uint32_t unique_id) {
    uril_cli_pending_op_t* ret = NULL;
    size_t i = 0;

    for (i = 0; i < ARRAY_SIZE(context.pending_ops) && ret == NULL; i++) {
        if (context.pending_ops[i].unique_id == unique_id) {
            ret = &(context.pending_ops[i]);
        }
    }
    return ret;
}

static int handle_receive_ind(uril_protocol_msg_t* ind) {
    URIL_CLI_TRACE_D("handle_receive_ind()");
    int ret = -1;

    assert(ind != NULL);
    if (context.ind_cb != NULL) {
        context.ind_cb(context.handle, ind->sim_id, ind->msg_id);
    }
    return ret;
}

static int handle_receive_rsp(uril_protocol_msg_t* resp) {
    URIL_CLI_TRACE_D("handle_receive_rsp()");
    int ret = -1;

    assert(resp != NULL);

    uril_cli_pending_op_t* op = NULL;
    const void* client_data = NULL;
    uril_client_async_cb async_resp_cb = NULL;

    if (cmd_mutex.lock(&cmd_mutex) != 0) {
        URIL_CLI_TRACE_E("failed to lock cmd mutex");
        return -1;
    }
    op = lookup_pending_op(resp->unique_id);
    if (op == NULL) {
        URIL_CLI_TRACE_E("op lookup failed");
    } else {
        URIL_CLI_TRACE_D("found pending op: %d", op->unique_id);
        op->busy = 0;
        async_resp_cb = op->async_resp_cb;
        client_data = op->client_data;
    }
    if (cmd_mutex.unlock(&cmd_mutex) != 0) {
        URIL_CLI_TRACE_E("failed to unlock cmd mutex");
        return -1;
    }

    if (async_resp_cb != NULL) {
        URIL_CLI_TRACE_D("invoking async_resp_cb()");
        async_resp_cb(context.handle, resp->sim_id, resp->msg_id, resp->payload,
                sizeof(resp->payload), client_data);
        ret = 0;
    } else {
        URIL_CLI_TRACE_E("async_resp_cb is not set");
    }
    return ret;
}

static int connector_on_receive(const service_connector_t* connector,
        size_t len, const void* data) {

    URIL_CLI_TRACE_D("connector_on_receive(%zu, ...)", len);
    if (len < sizeof(uril_protocol_msg_t)) {
        URIL_CLI_TRACE_E("insufficient length received");
        return -1;
    }
    uril_protocol_msg_t msg;
    uril_protocol_msg_from_buffer(&msg, data, len);

    URIL_CLI_TRACE_D("connector_on_receive: msg_id = %d, is_ind = %d",
            msg.msg_id, msg.is_indication);

    if (msg.is_indication) {
        return handle_receive_ind(&msg);
    } else {
        return handle_receive_rsp(&msg);
    }
}

static int connector_on_disconnect(const service_connector_t* connector,
        uint32_t error) {
    return -1;
}

uint32_t uril_client_init(uril_client_handle_t* hndl,
        uril_client_ind_cb ind_cb,
        uril_client_async_cb default_resp_cb) {
    URIL_CLI_TRACE_D("uril_client_init(...)");

    if (service_connector == NULL) {
        service_connector = unix_service_connector_create();
    }

    if (hndl == NULL) {
        URIL_CLI_TRACE_E("invalid handle ptr");
        return -1;
    }

    if (service_connector == NULL) {
        URIL_CLI_TRACE_E("Service connector is NULL");
        return -1;
    }

    memset(&context, 0, sizeof(context));

    context.ind_cb = ind_cb;
    context.default_async_cb = default_resp_cb;

    int attempts_count = 0;
    int connection_successful = 0;

    while (attempts_count++ < URIL_CONNECT_ATTEMPTS_COUNT && !connection_successful) {
        connection_successful = service_connector->connect(service_connector, connector_on_receive,
                connector_on_disconnect) == 0;
        if (!connection_successful) {
            URIL_CLI_TRACE_E("failed to connect to uril service, attempt %d / %d",
                    attempts_count, URIL_CONNECT_ATTEMPTS_COUNT);
            sleep(1);
        } else {
            URIL_CLI_TRACE_D("connection to uril service successful");
        }
    }
    if (!connection_successful) {
        return -1;
    }

    context.initialized = 1;

    if (s_init_mutex.lock(&s_init_mutex) == 0) {
        context.handle = ++s_unique_handle;
        *hndl = context.handle;
        s_init_mutex.unlock(&s_init_mutex);
    } else {
        URIL_CLI_TRACE_E("failed to lock init mutex");
        return -1;
    }
    URIL_CLI_TRACE_D("handle successfully allocated: %d", *hndl);

    return 0;
}

uint32_t uril_client_execute_command_async(uril_client_handle_t hndl,
        uint8_t sim_id, uint32_t msg_id, const void* req, size_t req_len,
        uril_client_async_cb async_resp_cb, const void* client_data) {

    URIL_CLI_TRACE_D(
            "uril_client_execute_command_async(%d, %d, %x, ...)", hndl,
            sim_id, msg_id);

    if (!context.initialized) {
        URIL_CLI_TRACE_E("context not initialized");
        return -1;
    }
    if (hndl != context.handle) {
        URIL_CLI_TRACE_E("invalid handle");
        return -1;
    }
    if (req == NULL) {
        URIL_CLI_TRACE_E("invalid req");
        return -1;
    }

    uril_protocol_msg_t msg = { .sim_id = sim_id, .msg_id = msg_id };
    msg.len = (uint16_t) req_len;
    memcpy(msg.payload, req, req_len);

    if (s_unique_id_gen_mutex.lock(&s_unique_id_gen_mutex) == 0) {
        msg.unique_id = ++s_unique_msg_id;
        s_unique_id_gen_mutex.unlock(&s_unique_id_gen_mutex);
    } else {
        URIL_CLI_TRACE_E("failed to lock unique id gen mutex");
        return -1;
    }
    msg_print_no_check(&msg);

    uril_cli_pending_op_t* op = NULL;

    if (cmd_mutex.lock(&cmd_mutex) != 0) {
        URIL_CLI_TRACE_E("failed to lock cmd mutex");
        return -1;
    }
    op = acquire_free_op_slot();
    if (cmd_mutex.unlock(&cmd_mutex) != 0) {
        URIL_CLI_TRACE_E("failed to unlock cmd mutex");
        return -1;
    }
    if (op == NULL) {
        URIL_CLI_TRACE_E("no free op slot found");
        return -1;
    }
    if (async_resp_cb != NULL) {
        op->async_resp_cb = async_resp_cb;
    } else {
        URIL_CLI_TRACE_D("using default callback");
        op->async_resp_cb = context.default_async_cb;
    }
    op->client_data = client_data;
    op->unique_id = msg.unique_id;

    URIL_CLI_TRACE_D("sending request with unique id = %d", op->unique_id);

    return service_connector->send(service_connector, sizeof(msg), &msg);
}

static void async_cb(uril_client_handle_t hndl, uint8_t sim_id,
        uint32_t msg_id, const void* resp, size_t resp_len,
        const void* client_data) {
    URIL_CLI_TRACE_D("async_cb(%d, %d, %x, ...)", hndl, sim_id, msg_id);

    struct async_params* params = (struct async_params*) client_data;
    if (params != NULL) {
        memcpy(params->resp, resp, MIN(params->resp_len, resp_len));
        params->success = 1;
        URIL_CLI_TRACE_D("signaling condition");
        params->mutex->lock(params->mutex);
        params->condition = 1;
        params->mutex->unlock(params->mutex);
        params->signal_cond->signal(params->signal_cond);
    } else {
        URIL_CLI_TRACE_E("unexpected client data");
    }
}

uint32_t uril_client_execute_command_sync(uril_client_handle_t hndl,
        uint8_t sim_id, uint32_t msg_id, const void* req, size_t req_len,
        void* resp, size_t resp_len, uint32_t timeout_ms) {

    URIL_CLI_TRACE_D(
            "uril_client_execute_command_sync(%d, %d, %x, ...)", hndl,
            sim_id, msg_id);

    uint32_t ret = -1;
    osal_mutex_t sync_mutex = UNIX_MUTEX;
    osal_condition_t condition = UNIX_CONDITION;

    if (sync_mutex.init(&sync_mutex) != 0) {
        URIL_CLI_TRACE_E("sync mutex init failed");
        goto EXIT;
    }

    if (condition.init(&condition) != 0) {
        URIL_CLI_TRACE_E("sync condition init failed");
        goto EXIT;
    }
    struct async_params params = { .signal_cond = &condition, .mutex =
            &sync_mutex, .condition = 0, .resp = resp, .resp_len = resp_len };

    URIL_CLI_TRACE_D("calling async request");

    if (uril_client_execute_command_async(hndl, sim_id, msg_id, req,
            req_len, async_cb, &params) != 0) {
        URIL_CLI_TRACE_E("async operation failed");
        goto EXIT;
    }
    if (sync_mutex.lock(&sync_mutex) != 0) {
        URIL_CLI_TRACE_E("failed to lock cmd mutex");
        goto EXIT;
    }
    if (params.condition == 0) {
        int cond_ret = condition.timedwait(&condition, &sync_mutex, timeout_ms);
        if (cond_ret != 0) {
            URIL_CLI_TRACE_E("condition timed out: %d", cond_ret);
            goto EXIT;
        } else {
            URIL_CLI_TRACE_D("condition signaled");
            URIL_CLI_TRACE_D("cparams.condition = %d", params.condition);
        }
    }
    if (params.condition == 1) {
        ret = 0;
    }
    sync_mutex.unlock(&sync_mutex);

    EXIT: condition.destroy(&condition);
    sync_mutex.destroy(&sync_mutex);

    return ret;
}

uint32_t uril_client_release(uril_client_handle_t hndl) {
    URIL_CLI_TRACE_D("uril_client_release(%d)", hndl);

    if (context.initialized) {
        context.ind_cb = NULL;
        context.default_async_cb = NULL;
        context.initialized = 0;
        return 0;
    } else {
        URIL_CLI_TRACE_E("failed release handle %d: not initialized", hndl);
    }
    return -1;
}

