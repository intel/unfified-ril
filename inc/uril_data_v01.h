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

#ifndef URIL_DATA_V01_H_
#define URIL_DATA_V01_H_

#include <stdint.h>
#include "uril_common_v01.h"
#include "uril_nw_v01.h"

#ifdef __cplusplus
extern "C" {
#endif

#define URIL_DATA_MAX_APN_LEN_V01 150
#define URIL_DATA_MAX_USERNAME_LEN_V01 127
#define URIL_DATA_MAX_PASSWORD_LEN_V01 127
#define URIL_DATA_MAX_ADDR_LEN_V01 256
#define URIL_DATA_MAX_DEVICE_NAME_LEN_V01 13
#define URIL_DATA_MAX_ADDR_COUNT_V01 4
#define URIL_DATA_MAX_PROFILES_COUNT_V01 8
#define URIL_DATA_MAX_CALLS_COUNT_V01 6

typedef enum {
    URIL_DATA_PDP_TYPE_NONE = 0,
    URIL_DATA_PDP_TYPE_IP = 1,
    URIL_DATA_PDP_TYPE_IPV6 = 2,
    URIL_DATA_PDP_TYPE_IPV4V6 = 3,
    URIL_DATA_PDP_TYPE_PPP = 4
} uril_data_pdp_type_t_v01;

typedef enum {
    URIL_DATA_AUTH_TYPE_NONE = 0,
    URIL_DATA_AUTH_TYPE_PAP = 1,
    URIL_DATA_AUTH_TYPE_CHAP = 2,
    URIL_DATA_AUTH_TYPE_PAPCHAP = 3
} uril_data_auth_type_t_v01;

typedef enum {
    URIL_DATA_BEARER_TECH_TYPE_UNKNOWN_V01 = 0x00,
    URIL_DATA_BEARER_TECH_TYPE_CDMA_1X_V01 = 0x01,
    URIL_DATA_BEARER_TECH_TYPE_EVDO_REV0_V01 = 0x02,
    URIL_DATA_BEARER_TECH_TYPE_EVDO_REVA_V01 = 0x03,
    URIL_DATA_BEARER_TECH_TYPE_EVDO_REVB_V01 = 0x04,
    URIL_DATA_BEARER_TECH_TYPE_EHRPD_V01 = 0x05,
    URIL_DATA_BEARER_TECH_TYPE_FMC_V01 = 0x06,
    URIL_DATA_BEARER_TECH_TYPE_HRPD_V01 = 0x07,
    URIL_DATA_BEARER_TECH_TYPE_3GPP2_WLAN_V01 = 0x08,
    URIL_DATA_BEARER_TECH_TYPE_WCDMA_V01 = 0x09,
    URIL_DATA_BEARER_TECH_TYPE_GPRS_V01 = 0x0A,
    URIL_DATA_BEARER_TECH_TYPE_HSDPA_V01 = 0x0B,
    URIL_DATA_BEARER_TECH_TYPE_HSUPA_V01 = 0x0C,
    URIL_DATA_BEARER_TECH_TYPE_EDGE_V01 = 0x0D,
    URIL_DATA_BEARER_TECH_TYPE_LTE_V01 = 0x0E,
    URIL_DATA_BEARER_TECH_TYPE_HSDPA_PLUS_V01 = 0x0F,
    URIL_DATA_BEARER_TECH_TYPE_DC_HSDPA_PLUS_V01 = 0x10,
    URIL_DATA_BEARER_TECH_TYPE_HSPA_V01 = 0x11,
    URIL_DATA_BEARER_TECH_TYPE_64_QAM_V01 = 0x12,
    URIL_DATA_BEARER_TECH_TYPE_TDSCDMA_V01 = 0x13,
    URIL_DATA_BEARER_TECH_TYPE_GSM_V01 = 0x14,
    URIL_DATA_BEARER_TECH_TYPE_3GPP_WLAN_V01 = 0x15,
} uril_data_bearer_tech_info_t_v01;

typedef enum {
    URIL_DATA_FAIL_CAUSE_NONE = 0,
    URIL_DATA_FAIL_CAUSE_OPERATOR_BARRED = 0x08,
    URIL_DATA_FAIL_CAUSE_INSUFFICIENT_RESOURCES = 0x1A,
    URIL_DATA_FAIL_CAUSE_MISSING_UKNOWN_APN = 0x1B,
    URIL_DATA_FAIL_CAUSE_UNKNOWN_PDP_ADDRESS_TYPE = 0x1C,
    URIL_DATA_FAIL_CAUSE_USER_AUTHENTICATION = 0x1D,
    URIL_DATA_FAIL_CAUSE_ACTIVATION_REJECT_GGSN = 0x1E,
    URIL_DATA_FAIL_CAUSE_ACTIVATION_REJECT_UNSPECIFIED = 0x1F,
    URIL_DATA_FAIL_CAUSE_SERVICE_OPTION_NOT_SUPPORTED = 0x20,
    URIL_DATA_FAIL_CAUSE_SERVICE_OPTION_NOT_SUBSCRIBED = 0x21,
    URIL_DATA_FAIL_CAUSE_SERVICE_OPTION_OUT_OF_ORDER = 0x22,
    URIL_DATA_FAIL_CAUSE_NSAPI_IN_USE = 0x23,
    URIL_DATA_FAIL_CAUSE_REGULAR_DEACTIVATION = 0x24,
    URIL_DATA_FAIL_CAUSE_ONLY_IPV4_ALLOWED = 0x32,
    URIL_DATA_FAIL_CAUSE_ONLY_IPV6_ALLOWED = 0x33,
    URIL_DATA_FAIL_CAUSE_ONLY_SINGLE_BEARER_ALLOWED = 0x34,
    URIL_DATA_FAIL_CAUSE_PROTOCOL_ERRORS = 0x6F,
    URIL_DATA_FAIL_CAUSE_VOICE_REGISTRATION_FAIL = -1,
    URIL_DATA_FAIL_CAUSE_DATA_REGISTRATION_FAIL = -2,
    URIL_DATA_FAIL_CAUSE_SIGNAL_LOST = -3,
    URIL_DATA_FAIL_CAUSE_PREF_RADIO_TECH_CHANGED = -4,
    URIL_DATA_FAIL_CAUSE_RADIO_POWER_OFF = -5,
    URIL_DATA_FAIL_CAUSE_TETHERED_CALL_ACTIVE = -6,
    URIL_DATA_FAIL_CAUSE_ERROR_UNSPECIFIED = 0xffff,
} uril_data_call_failure_cause_t_v01;

typedef enum {
    URIL_DATA_PHYSLINK_STATE_INACTIVE_V01 = 0,
    URIL_DATA_PHYSLINK_STATE_ACTIVE_DOWN_V01 = 1,
    URIL_DATA_PHYSLINK_STATE_ACTIVE_UP_V01 = 2,
} uril_physical_link_state_t_v01;

typedef enum {
    URIL_DATA_END_REASON_NONE = 0, URIL_DATA_END_REASON_RADIO_OFF = 1
} uril_data_call_end_reason_type_t_v01;

typedef struct {
    uint8_t profile_id;
    char apn[URIL_DATA_MAX_APN_LEN_V01];
    uril_data_pdp_type_t_v01 pdp_type;
    uril_data_auth_type_t_v01 auth_type;
    char user[URIL_DATA_MAX_USERNAME_LEN_V01];
    char password[URIL_DATA_MAX_PASSWORD_LEN_V01];
    uril_nw_tech_t_v01 tech;
    uint8_t enabled;
} __attribute__((packed)) uril_data_profile_info_t_v01;

typedef struct {
    char addr[URIL_DATA_MAX_ADDR_LEN_V01];
    uint32_t prefix_len;
} __attribute__((packed)) uril_data_addr_t_v01;

typedef struct {
    uril_data_call_failure_cause_t_v01 failure_cause;
    uint16_t suggested_retry_time;
    uint8_t cid;
    uril_physical_link_state_t_v01 phy_link_state;
    uril_data_pdp_type_t_v01 pdp_type;
    char interface_name[URIL_DATA_MAX_DEVICE_NAME_LEN_V01];
    uint32_t pdp_addrs_len;
    uril_data_addr_t_v01 pdp_addrs[URIL_DATA_MAX_ADDR_COUNT_V01];
    uril_data_addr_t_v01 gtwy_addr;
    uril_data_addr_t_v01 dnsp_addr;
    uril_data_addr_t_v01 dnss_addr;
    uril_data_addr_t_v01 pcscf_addr;
    uint16_t mtu;
} __attribute__((packed)) uril_data_call_info_t_v01;

typedef struct {
    uint8_t profiles_len;
    uril_data_profile_info_t_v01 profiles[URIL_DATA_MAX_PROFILES_COUNT_V01];
} __attribute__((packed)) uril_data_set_data_profiles_req_msg_v01;

typedef struct {
    uril_response_t_v01 response;
} __attribute__((packed)) uril_data_set_data_profiles_resp_msg_v01;

typedef struct {
    uril_data_bearer_tech_info_t_v01 rat;
    uint8_t profile_id;
    char apn[URIL_DATA_MAX_APN_LEN_V01];
    uril_data_pdp_type_t_v01 pdp_type;
    uril_data_auth_type_t_v01 auth_type;
    char user[URIL_DATA_MAX_USERNAME_LEN_V01];
    char password[URIL_DATA_MAX_PASSWORD_LEN_V01];
} __attribute__((packed)) uril_data_setup_data_call_req_msg_v01;

typedef struct {
    uril_response_t_v01 response;
    uril_data_call_info_t_v01 data_call;
} __attribute__((packed)) uril_data_setup_data_call_resp_msg_v01;

typedef struct {
    uint8_t cid;
    uril_data_call_end_reason_type_t_v01 reason;
} __attribute__((packed)) uril_data_deactivate_data_call_req_msg_v01;

typedef struct {
    uril_response_t_v01 response;
} __attribute__((packed)) uril_data_deactivate_data_call_resp_msg_v01;

typedef struct {
    char __placeholder;
} __attribute__((packed)) uril_data_get_data_call_list_req_msg_v01;

typedef struct {
    uril_response_t_v01 response;
    uint8_t data_calls_len;
    uril_data_call_info_t_v01 data_calls[URIL_DATA_MAX_CALLS_COUNT_V01];
} __attribute__((packed)) uril_data_get_data_call_list_resp_msg_v01;

typedef struct {
    char apn[URIL_DATA_MAX_APN_LEN_V01];
    uril_data_pdp_type_t_v01 pdp_type;
    uril_data_auth_type_t_v01 auth_type;
    char user[URIL_DATA_MAX_USERNAME_LEN_V01];
    char password[URIL_DATA_MAX_PASSWORD_LEN_V01];
} __attribute__((packed)) uril_data_set_initial_params_req_msg_v01;

typedef struct {
    uril_response_t_v01 response;
} __attribute__((packed)) uril_data_set_initial_params_resp_msg_v01;

#define URIL_DATA_SET_DATA_PROFILES_REQ_V01 0x0600
#define URIL_DATA_SET_DATA_PROFILES_RESP_V01 0x0600
#define URIL_DATA_SETUP_DATA_CALL_REQ_V01 0x0601
#define URIL_DATA_SETUP_DATA_CALL_RESP_V01 0x0601
#define URIL_DATA_DEACTIVATE_DATA_CALL_REQ_V01 0x0602
#define URIL_DATA_DEACTIVATE_DATA_CALL_RESP_V01 0x0602
#define URIL_DATA_GET_DATA_CALL_LIST_REQ_V01 0x0603
#define URIL_DATA_GET_DATA_CALL_LIST_RESP_V01 0x0603
#define URIL_DATA_SET_INITIAL_ATTACH_PARAMS_REQ_V01 0x0604
#define URIL_DATA_SET_INITIAL_ATTACH_PARAMS_RESP_V01 0x0604
#define URIL_DATA_CALL_LIST_CHANGED_EVENT_IND_V01 0x0620

#ifdef __cplusplus
}
#endif

#endif /* URIL_DATA_V01_H_ */
