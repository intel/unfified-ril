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

#ifndef URIL_COMMON_V01_H_
#define URIL_COMMON_V01_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    URIL_RESULT_SUCCESS_V01 = 0, URIL_RESULT_FAILURE_V01 = 1,
} uril_result_t_v01;

typedef enum {
    URIL_ERROR_SUCCESS_V01 = 0,
    URIL_ERROR_RADIO_NOT_AVAILABLE_V01 = 1,
    URIL_ERROR_GENERIC_FAILURE_V01 = 2,
    URIL_ERROR_PASSWORD_INCORRECT_V01 = 3,
    URIL_ERROR_SIM_PIN2_V01 = 4,
    URIL_ERROR_SIM_PUK2_V01 = 5,
    URIL_ERROR_REQUEST_NOT_SUPPORTED_V01 = 6,
    URIL_ERROR_CANCELLED_V01 = 7,
    URIL_ERROR_OP_NOT_ALLOWED_DURING_VOICE_CALL_V01 = 8,

    URIL_ERROR_OP_NOT_ALLOWED_BEFORE_REG_TO_NW_V01 = 9,

    URIL_ERROR_SMS_SEND_FAIL_RETRY_V01 = 10,
    URIL_ERROR_SIM_ABSENT_V01 = 11,

    URIL_ERROR_SUBSCRIPTION_NOT_AVAILABLE_V01 = 12,

    URIL_ERROR_MODE_NOT_SUPPORTED_V01 = 13,
    URIL_ERROR_FDN_CHECK_FAILURE_V01 = 14,
    URIL_ERROR_ILLEGAL_SIM_OR_ME_V01 = 15,

    URIL_ERROR_MISSING_RESOURCE_V01 = 16,
    URIL_ERROR_NO_SUCH_ELEMENT_V01 = 17,
    URIL_ERROR_DIAL_MODIFIED_TO_USSD_V01 = 18,
    URIL_ERROR_DIAL_MODIFIED_TO_SS_V01 = 19,
    URIL_ERROR_DIAL_MODIFIED_TO_DIAL_V01 = 20,

    URIL_ERROR_USSD_MODIFIED_TO_DIAL_V01 = 21,
    URIL_ERROR_USSD_MODIFIED_TO_SS_V01 = 22,
    URIL_ERROR_USSD_MODIFIED_TO_USSD_V01 = 23,

    URIL_ERROR_SS_MODIFIED_TO_DIAL_V01 = 24,
    URIL_ERROR_SS_MODIFIED_TO_USSD_V01 = 25,
    URIL_ERROR_SUBSCRIPTION_NOT_SUPPORTED_V01 = 26,
    URIL_ERROR_SS_MODIFIED_TO_SS_V01 = 27,
    URIL_ERROR_LCE_NOT_SUPPORTED_V01 = 36,

    URIL_ERROR_UNSUPPORTED_V01 = 500,
    URIL_ERROR_OEM_ERROR_OFFSET_V01 = 501
} uril_error_t_v01;

typedef struct {
    uril_result_t_v01 result;
    uril_error_t_v01 error;
} __attribute__((packed)) uril_response_t_v01;

#ifdef __cplusplus
}
#endif
#endif /* URIL_COMMON_V01_H_ */
