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
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>

#include "../uril_cli_log.h"
#include "uril_protocol.h"
#include "unix_socket_service_connector.h"
#include "service_connector.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(*a))

#define UNIX_SOCKET_NAME    "/tmp/uril_sock"

typedef struct {
    int sock_fd;
    pthread_t thread;
} unix_socket_connector_ctx_t;

static void* receive_routine(void* args) {
    URIL_CLI_TRACE_D("receive_routine()");

    service_connector_t* connector = (service_connector_t*) args;
    assert(connector != NULL);

    unix_socket_connector_ctx_t* ctx =
            (unix_socket_connector_ctx_t*) connector->ctx;
    assert(connector->ctx != NULL);

    while (1) {
        uril_protocol_msg_t msg;
        memset(&msg, 0xff, sizeof(msg));

        URIL_CLI_TRACE_D("waiting for incoming data");
        ssize_t received_count = recv(ctx->sock_fd, &msg, sizeof(msg), MSG_WAITALL);

        if (received_count < 0) {
            URIL_CLI_TRACE_E("recv failed: %s", strerror(errno));
            break;
        }
        if (received_count == 0) {
            URIL_CLI_TRACE_E("service disconnected");
            connector->on_disconnect(connector, -1);
            break;
        } else {
            URIL_CLI_TRACE_D("received %zd bytes", received_count);
#if 0
            URIL_CLI_TRACE_D("DUMP CLIENT");
            char tempHex[sizeof(uril_protocol_msg_t) * 2 + 1];
            char* buf_ptr = tempHex;

            memset(tempHex, 0, sizeof(tempHex));

            for (size_t i = 0; i < sizeof(uril_protocol_msg_t); i++) {
                uint8_t* val = ((uint8_t*) &msg) + i;
                buf_ptr += sprintf(buf_ptr, "%02X", *val);
            }
            *(buf_ptr) = '\0';
            URIL_CLI_TRACE_D("\r%s\r", tempHex);
#endif
            connector->on_receive(connector, received_count, &msg);
        }
        usleep(1000); // prevent from CPU overload in case of abnormal incoming data
    }
    return NULL;
}

const char* unix_socket_service_connector_get_name_routine() {
    return "UnixSocketServiceConnector";
}

int unix_socket_service_connector_connect_routine(
        service_connector_t* connector,
        service_connector_receive_cb_t receive_cb,
        service_connector_disconnect_cb_t disconnect_cb) {
    URIL_CLI_TRACE_D("unix_socket_service_connector_connect_routine()");

    assert(connector != NULL);
    assert(receive_cb != NULL);
    assert(disconnect_cb != NULL);

    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
        URIL_CLI_TRACE_E("Could not create socket: %s", strerror(errno));
        return -1;
    }
    URIL_CLI_TRACE_D("Socket created");

    unix_socket_connector_ctx_t* ctx = (unix_socket_connector_ctx_t*) malloc(
            sizeof(unix_socket_connector_ctx_t));
    if (ctx == NULL) {
        URIL_CLI_TRACE_E("Could not allocate ctx");
        close(sock);
        return -1;
    }
    connector->ctx = ctx;
    ctx->sock_fd = sock;
    connector->on_disconnect = disconnect_cb;
    connector->on_receive = receive_cb;

    struct sockaddr_un serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sun_family = AF_UNIX;
    strcpy(serveraddr.sun_path, UNIX_SOCKET_NAME);

    if (connect(ctx->sock_fd, (struct sockaddr*) &serveraddr,
            SUN_LEN(&serveraddr)) < 0) {
        URIL_CLI_TRACE_E("Could not connect socket: %s", strerror(errno));
        close(ctx->sock_fd);
        free(ctx);
        return -1;
    }
    URIL_CLI_TRACE_D("Socket connected");

    size_t protocol_len = sizeof(uril_protocol_msg_t);
    if (setsockopt(ctx->sock_fd, SOL_SOCKET, SO_RCVLOWAT,
            &protocol_len, protocol_len) < 0) {
        URIL_CLI_TRACE_E("setsockopt failed: %s", strerror(errno));
        close(ctx->sock_fd);
        free(ctx);
        return -1;
    }

    if (pthread_create(&(ctx->thread), NULL, receive_routine, connector) != 0) {
        URIL_CLI_TRACE_E("Could not create thread: %s", strerror(errno));
        close(ctx->sock_fd);
        free(ctx);
        return -1;
    }
    URIL_CLI_TRACE_D("Thread started");
    return 0;
}

int unix_socket_service_connector_disconnect_routine(
        service_connector_t* connector) {
    URIL_CLI_TRACE_D("unix_socket_service_connector_disconnect_routine()");

    assert(connector != NULL);
    assert(connector->ctx != NULL);

    unix_socket_connector_ctx_t* ctx =
            (unix_socket_connector_ctx_t*) connector->ctx;
    if (ctx->sock_fd >= 0) {
        close(ctx->sock_fd);
        if (pthread_join(ctx->thread, NULL) != 0) {
            URIL_CLI_TRACE_E("thread join failed");
        }
        free(ctx);
        connector->ctx = NULL;
        connector->on_disconnect = NULL;
        connector->on_receive = NULL;
    } else {
        URIL_CLI_TRACE_E("Invalid socket, was connect called first?");
        return -1;
    }
    return 0;
}

int unix_socket_service_connector_send_routine(
        const service_connector_t* connector, size_t len, const void* buff) {
    URIL_CLI_TRACE_D("unix_socket_service_connector_send_routine()");

    assert(connector != NULL);
    assert(connector->ctx != NULL);
    assert(buff != NULL);

    unix_socket_connector_ctx_t* ctx =
            (unix_socket_connector_ctx_t*) connector->ctx;

    if (send(ctx->sock_fd, buff, len, MSG_DONTWAIT) < 0) {
        URIL_CLI_TRACE_E("send failed: %s", strerror(errno));
        return -1;
    }
    return 0;
}

service_connector_t* unix_service_connector_create() {
    service_connector_t* ret = (service_connector_t*) malloc(
            sizeof(service_connector_t));
    URIL_CLI_TRACE_D("unix_service_connector_create()");

    if (ret != NULL) {
        ret->ctx = NULL;
        ret->get_name = unix_socket_service_connector_get_name_routine;
        ret->connect = unix_socket_service_connector_connect_routine;
        ret->disconnect = unix_socket_service_connector_disconnect_routine;
        ret->send = unix_socket_service_connector_send_routine;
        ret->on_receive = NULL;
        ret->on_disconnect = NULL;
    }
    return ret;
}
