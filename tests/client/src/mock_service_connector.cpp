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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "service_connector.h"
#include "mock_service_connector.h"
#include "uril_protocol.h"
#include "uril_nw_v01.h"

const char* mock_service_connector_get_name_routine() {
    return "MockServiceConnector";
}

int mock_service_connector_connect_routine(service_connector_t* connector,
        service_connector_receive_cb_t receive_cb,
        service_connector_disconnect_cb_t disconnect_cb) {
    puts("mock_service_connector_connect_routine()");

    assert(connector != NULL);
    assert(receive_cb != NULL);
    assert(disconnect_cb != NULL);

    connector->on_receive = receive_cb;
    connector->on_disconnect = disconnect_cb;

    return 0;
}

int mock_service_connector_disconnect_routine(service_connector_t* connector) {
    puts("mock_service_connector_disconnect_routine()");
    return 0;
}

int mock_service_connector_send_routine(const service_connector_t* connector,
        size_t len, const void* buff) {
    puts("mock_service_connector_send_routine()");

    uril_protocol_msg_t* msg = (uril_protocol_msg_t*) buff;

    if (msg == nullptr) {
        puts("msg is null");
        return -1;
    }

    uril_protocol_msg_t resp;
    resp.unique_id = msg->unique_id;
    resp.sim_id = msg->sim_id;

    switch (msg->msg_id) {
    case URIL_NW_GET_CELL_ACCESS_STATE_REQ_V01: {
        puts("URIL_NW_GET_CELL_ACCESS_STATE_REQ_V01");
        resp.msg_id = URIL_NW_GET_CELL_ACCESS_STATE_RESP_V01;
        resp.is_indication = 0;
        uril_nw_get_cell_access_state_resp_msg_v01 cell_access_resp;
        cell_access_resp.nw_cell_access_state =
                URIL_NW_CELL_ACCESS_ALL_CALLS_V01;
        cell_access_resp.nw_cell_access_state_follows = 1;
        cell_access_resp.response.result = URIL_RESULT_SUCCESS_V01;
        memcpy(resp.payload, &cell_access_resp, sizeof(cell_access_resp));
    }
        break;
    default:
        uril_response_t_v01 error;
        error.result = URIL_RESULT_FAILURE_V01;
        error.error = URIL_ERROR_OEM_ERROR_OFFSET_V01;
        memcpy(resp.payload, &error, sizeof(error));
    }
    connector->on_receive(connector, sizeof(resp), &resp);

    return 0;
}

service_connector_t* mock_service_connector_create() {
    service_connector_t* ret = (service_connector_t*) malloc(
            sizeof(service_connector_t));

    if (ret != NULL) {
        ret->ctx = NULL;
        ret->get_name = mock_service_connector_get_name_routine;
        ret->connect = mock_service_connector_connect_routine;
        ret->disconnect = mock_service_connector_disconnect_routine;
        ret->send = mock_service_connector_send_routine;
        ret->on_receive = NULL;
        ret->on_disconnect = NULL;
    }
    return ret;
}
