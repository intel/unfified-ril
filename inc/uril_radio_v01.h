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

#ifndef URIL_RADIO_V01_H_
#define URIL_RADIO_V01_H_

#include <stdint.h>
#include "uril_common_v01.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    URIL_RADIO_MODE_OFFLINE_V01 = 0x0001,
    URIL_RADIO_MODE_ONLINE_V01 = 0x0002,
    URIL_RADIO_MODE_UNAVAILABLE_V01 = 0x0003,
} uril_radio_mode_t_v01;

typedef struct {
    char __placeholder;
} __attribute__((packed)) uril_radio_get_radio_mode_req_msg_v01;

typedef struct {
    uril_response_t_v01 response;
    uint8_t radio_mode_follows;
    uril_radio_mode_t_v01 radio_mode;
} __attribute__((packed)) uril_radio_get_radio_mode_resp_msg_v01;

typedef struct {
    uril_radio_mode_t_v01 radio_mode;
} __attribute__((packed)) uril_radio_set_radio_mode_req_msg_v01;

typedef struct {
    uril_response_t_v01 response;
} __attribute__((packed))uril_radio_set_radio_mode_resp_msg_v01;

#define URIL_RADIO_GET_RADIO_MODE_REQ_V01 0x0101
#define URIL_RADIO_GET_RADIO_MODE_RESP_V01 0x0101
#define URIL_RADIO_SET_RADIO_MODE_REQ_V01 0x0102
#define URIL_RADIO_SET_RADIO_MODE_RESP_V01 0x0102
#define URIL_RADIO_MODE_CHANGED_EVENT_IND_V01 0x0120

#ifdef __cplusplus
}
#endif

#endif /* URIL_RADIO_V01_H_ */
