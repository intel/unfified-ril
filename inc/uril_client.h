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

#ifndef URIL_CLIENT_H_
#define URIL_CLIENT_H_

#include <stdint.h>
#include <stddef.h>

#include "uril_common_v01.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t uril_client_handle_t;

typedef void (*uril_client_async_cb)(uril_client_handle_t hndl,
        uint8_t sim_id, uint32_t msg_id, const void* resp, size_t resp_len,
        const void* client_data);

typedef void (*uril_client_ind_cb)(uril_client_handle_t hndl,
        uint8_t sim_id, uint32_t msg_id);

uint32_t uril_client_init(uril_client_handle_t* hndl,
        uril_client_ind_cb ind_cb,
        uril_client_async_cb default_resp_cb);

uint32_t uril_client_execute_command_async(uril_client_handle_t hndl,
        uint8_t sim_id, uint32_t msg_id, const void* req, size_t req_len,
        uril_client_async_cb async_resp_cb, const void* client_data);

#define URIL_CLIENT_EXECUTE_COMMAND_ASYNC(hndl, sim_id, msg_id, req_ptr, cb, client_data) \
        uril_client_execute_command_async(hndl,                                           \
    sim_id,                                                                                    \
    msg_id,                                                                                    \
    req_ptr,                                                                                   \
    sizeof(*req_ptr),                                                                           \
    cb,                                                                                        \
    client_data)

#define URIL_CLIENT_EXECUTE_COMMAND_NO_PAYLOAD_ASYNC(hndl,        \
                                                          sim_id,      \
                                                          msg_id,      \
                                                          req_ptr,     \
                                                          cb,          \
                                                          client_data) \
        uril_client_execute_command_async(hndl,                   \
    sim_id,                                                            \
    msg_id,                                                            \
    NULL,                                                              \
    0,                                                                 \
    cb,                                                                \
    client_data)

uint32_t uril_client_execute_command_sync(uril_client_handle_t hndl,
        uint8_t sim_id, uint32_t msg_id, const void* req, size_t req_len,
        void* resp, size_t resp_len, uint32_t timeout_ms);

#define URIL_CLIENT_EXECUTE_COMMAND_SYNC(hndl, sim_id, msg_id, req_ptr, resp_ptr, timeout_ms) \
        uril_client_execute_command_sync(hndl,                                                \
    sim_id,                                                                                        \
    msg_id,                                                                                        \
    req_ptr,                                                                                       \
    sizeof(*(req_ptr)),                                                                            \
    resp_ptr,                                                                                      \
    sizeof(*(resp_ptr)),                                                                           \
    timeout_ms)

#define URIL_CLIENT_EXECUTE_COMMAND_NO_PAYLOAD_SYNC(hndl, sim_id, msg_id, resp_ptr, timeout_ms) \
        uril_client_execute_command_sync(hndl,                                                  \
    sim_id,                                                                                          \
    msg_id,                                                                                          \
    NULL,                                                                                            \
    0,                                                                                               \
    resp_ptr,                                                                                        \
    sizeof(*(resp_ptr)),                                                                             \
    timeout_ms)

uint32_t uril_client_release(uril_client_handle_t hndl);

#ifdef __cplusplus
}
#endif
#endif /* URIL_CLIENT_H_ */
