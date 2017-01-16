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

#ifndef URIL_VOICE_V01_H_
#define URIL_VOICE_V01_H_

#include <stdint.h>
#include "uril_common_v01.h"

#ifdef __cplusplus
extern "C" {
#endif

#define URIL_VOICE_MAX_CALLS_COUNT_V01 8
#define URIL_VOICE_MAX_NUMBER_LEN_V01 82

typedef enum {
    URIL_VOICE_CALL_STATE_ACTIVE = 0,
    URIL_VOICE_CALL_STATE_HOLDING = 1,
    URIL_VOICE_CALL_STATE_DIALING = 2,    /* MO call only */
    URIL_VOICE_CALL_STATE_ALERTING = 3,   /* MO call only */
    URIL_VOICE_CALL_STATE_INCOMING = 4,   /* MT call only */
    URIL_VOICE_CALL_STATE_WAITING = 5     /* MT call only */
} uril_voice_call_state_t_v01;

typedef enum {
    URIL_VOICE_CALL_ANSWER_REJECT = 0,
    URIL_VOICE_CALL_ANSWER_ACCEPT = 1
} uril_voice_call_answer_t_v01;

// as defined in TS 27.007 7.7 "+CLIR"
typedef enum {
    URIL_VOICE_CALL_CLIR_DEFAULT = 0,
    URIL_VOICE_CALL_CLIR_INVOCATION = 1,
    URIL_VOICE_CALL_CLIR_SUPPRESSION = 2
} uril_voice_call_clir_t_v01;

typedef struct {
    uril_voice_call_state_t_v01 state;
    uint8_t id;
    uint8_t is_multiparty;
    uint8_t is_mt;
    uint8_t is_voice;
    char number[URIL_VOICE_MAX_NUMBER_LEN_V01];
    uril_voice_call_clir_t_v01 clir;
} __attribute__((packed)) uril_voice_call_info_t_v01;

typedef struct {
    char number[URIL_VOICE_MAX_NUMBER_LEN_V01];
    uril_voice_call_clir_t_v01 clir;
} __attribute__((packed)) uril_voice_call_start_req_msg_v01;

typedef struct {
    uril_response_t_v01 response;
} __attribute__((packed)) uril_voice_call_start_resp_msg_v01;

typedef struct {
    char __placeholder;
} __attribute__((packed)) uril_voice_get_call_list_req_msg_v01;

typedef struct {
    uril_response_t_v01 response;
    uint8_t voice_calls_len;
    uril_voice_call_info_t_v01 voice_calls[URIL_VOICE_MAX_CALLS_COUNT_V01];
} __attribute__((packed)) uril_voice_get_call_list_resp_msg_v01;

typedef struct {
    uint8_t call_id;
} __attribute__((packed)) uril_voice_call_stop_req_msg_v01;

typedef struct {
    uril_response_t_v01 response;
} __attribute__((packed)) uril_voice_call_stop_resp_msg_v01;

typedef struct {
    uint8_t call_id;
} __attribute__((packed)) uril_voice_call_answer_req_msg_v01;

typedef struct {
    uril_response_t_v01 response;
} __attribute__((packed)) uril_voice_call_answer_resp_msg_v01;

typedef struct {
    uint8_t auto_answer;
} __attribute__((packed)) uril_voice_call_set_auto_answer_req_msg_v01;

typedef struct {
    uril_response_t_v01 response;
} __attribute__((packed)) uril_voice_call_set_auto_answer_resp_msg_v01;

#define URIL_VOICE_CALL_START_REQ_V01 0x0400
#define URIL_VOICE_CALL_START_RESP_V01 0x0400
#define URIL_VOICE_GET_CALL_LIST_REQ_V01 0x0401
#define URIL_VOICE_GET_CALL_LIST_RESP_V01 0x0401
#define URIL_VOICE_CALL_STOP_REQ_V01 0x0402
#define URIL_VOICE_CALL_STOP_RESP_V01 0x0402
#define URIL_VOICE_CALL_ANSWER_REQ_V01 0x0403
#define URIL_VOICE_CALL_ANSWER_RESP_V01 0x0403
#define URIL_VOICE_CALL_SET_AUTO_ANSWER_REQ_V01 0x0404
#define URIL_VOICE_CALL_SET_AUTO_ANSWER_RESP_V01 0x0404
#define URIL_VOICE_CALL_LIST_CHANGED_EVENT_IND_V01 0x0420
#define URIL_VOICE_CALL_INCOMING_EVENT_IND_V01 0x0421

#ifdef __cplusplus
}
#endif

#endif /* URIL_VOICE_V01_H_ */
