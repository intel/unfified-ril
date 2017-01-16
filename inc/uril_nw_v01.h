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

#ifndef URIL_NW_V01_H_
#define URIL_NW_V01_H_

#include <stdint.h>
#include "uril_common_v01.h"

#ifdef __cplusplus
extern "C" {
#endif

#define URIL_NW_SCAN_MAX_ENTRY_COUNT_V01 32

typedef enum {
    URIL_NW_PREF_TYPE_GSM_WCDMA = 0, /* GSM/WCDMA (WCDMA preferred) */
    URIL_NW_PREF_TYPE_GSM_ONLY = 1, /* GSM only */
    URIL_NW_PREF_TYPE_WCDMA = 2, /* WCDMA  */
    URIL_NW_PREF_TYPE_GSM_WCDMA_AUTO = 3, /* GSM/WCDMA (auto mode, according to PRL) */
    URIL_NW_PREF_TYPE_CDMA_EVDO_AUTO = 4, /* CDMA and EvDo (auto mode, according to PRL) */
    URIL_NW_PREF_TYPE_CDMA_ONLY = 5, /* CDMA only */
    URIL_NW_PREF_TYPE_EVDO_ONLY = 6, /* EvDo only */
    URIL_NW_PREF_TYPE_GSM_WCDMA_CDMA_EVDO_AUTO = 7, /* GSM/WCDMA, CDMA, and EvDo (auto mode, according to PRL) */
    URIL_NW_PREF_TYPE_LTE_CDMA_EVDO = 8, /* LTE, CDMA and EvDo */
    URIL_NW_PREF_TYPE_LTE_GSM_WCDMA = 9, /* LTE, GSM/WCDMA */
    URIL_NW_PREF_TYPE_LTE_CMDA_EVDO_GSM_WCDMA = 10, /* LTE, CDMA, EvDo, GSM/WCDMA */
    URIL_NW_PREF_TYPE_LTE_ONLY = 11, /* LTE only */
    URIL_NW_PREF_TYPE_LTE_WCDMA = 12 /* LTE/WCDMA */
} uril_nw_pref_type_t_v01;

typedef enum {
    URIL_NW_SERVICE_NONE_V01 = 0,
    URIL_NW_SERVICE_LIMITED_V01 = 1,
    URIL_NW_SERVICE_FULL_V01 = 2
} uril_nw_service_t_v01;

typedef enum {
    URIL_NW_SELECTION_AUTOMATIC_V01 = 0,
    URIL_NW_SELECTION_MANUAL_V01 = 1
} uril_nw_selection_type_t_v01;

typedef enum {
    URIL_NW_NETWORK_STATUS_NONE_V01 = 0,
    URIL_NW_NETWORK_STATUS_CURRENT_SERVING_V01 = 1,
    URIL_NW_NETWORK_STATUS_AVAILABLE_V01 = 2,
    URIL_NW_NETWORK_STATUS_FORBIDDEN_V01 = 3,
} uril_nw_network_status_t_v01;

typedef enum {
    URIL_NW_RAT_NONE_V01 = 0,
    URIL_NW_RAT_TD_SCDMA_V01 = 1,
    URIL_NW_RAT_GSM_V01 = 2,
    URIL_NW_RAT_HSPAP_V01 = 3,
    URIL_NW_RAT_LTE_V01 = 4,
    URIL_NW_RAT_EHRPD_V01 = 5,
    URIL_NW_RAT_EVDO_B_V01 = 6,
    URIL_NW_RAT_HSPA_V01 = 7,
    URIL_NW_RAT_HSUPA_V01 = 8,
    URIL_NW_RAT_HSDPA_V01 = 9,
    URIL_NW_RAT_EVDO_A_V01 = 10,
    URIL_NW_RAT_EVDO_0_V01 = 11,
    URIL_NW_RAT_1xRTT_V01 = 12,
    URIL_NW_RAT_IS95B_V01 = 13,
    URIL_NW_RAT_IS95A_V01 = 14,
    URIL_NW_RAT_UMTS_V01 = 15,
    URIL_NW_RAT_EDGE_V01 = 16,
    URIL_NW_RAT_GPRS_V01 = 17,
    URIL_NW_RAT_IWLAN_V01 = 18
} uril_nw_rat_t_v01;

typedef enum {
    URIL_NW_CELL_ACCESS_NONE_V01 = 0,
    URIL_NW_CELL_ACCESS_NORMAL_ONLY_V01 = 1,
    URIL_NW_CELL_ACCESS_EMERGENCY_ONLY_V01 = 2,
    URIL_NW_CELL_ACCESS_NO_CALLS_V01 = 3,
    URIL_NW_CELL_ACCESS_ALL_CALLS_V01 = 4
} uril_nw_cell_access_state_t_v01;

typedef enum {
    URIL_NW_ROAMING_STATE_OFF_V01 = 0,
    URIL_NW_ROAMING_STATE_ON_V01 = 1
} uril_nw_roaming_state_t_v01;

typedef enum {
    URIL_DOMAIN_NONE_V01 = 0,
    URIL_DOMAIN_AUTO_V01 = 1,
    URIL_DOMAIN_CS_V01 = 2,
    URIL_DOMAIN_PS_V01 = 3,
} uril_nw_domain_t_v01;

typedef enum {
    URIL_NW_TECH_NONE_V01 = 0,
    URIL_NW_TECH_3GPP_V01 = 1,
} uril_nw_tech_t_v01;

// see 3GPP TS 24.008, 10.5.3.6 and Annex G.
typedef enum {
    URIL_NW_GENERAL = 0,
    URIL_NW_AUTHENTICATION_FAILURE = 1,
    URIL_NW_IMSI_UNKNOWN_IN_HLR = 2,
    URIL_NW_ILLEGAL_MS = 3,
    URIL_NW_ILLEGAL_ME = 4,
    URIL_NW_PLMN_NOT_ALLOWED = 5,
    URIL_NW_LOCATION_AREA_NOT_ALLOWED = 6,
    URIL_NW_ROAMING_NOT_ALLOWED = 7,
    URIL_NW_NO_SUITABLE_CELLS_IN_THIS_LOCATION_AREA = 8,
    URIL_NW_NETWORK_FAILURE = 9,
    URIL_NW_PERSISTENT_LOCATION_UPDATE_REJECT = 10,
    URIL_NW_MAC_FAILURE = 20,
    URIL_NW_SYNC_FAILURE = 21,
    URIL_NW_CONGESTION = 22,
    URIL_NW_GSM_AUTHENTICATION_UNACCEPTABLE = 23,
    URIL_NW_NOT_AUTHORIZED_FOR_THIS_CSG = 25,
    URIL_NW_SERVICE_OPTION_NOT_SUPPORTED = 32,
    URIL_NW_REQUESTED_SERVICE_OPTION_NOT_SUBSCRIBED = 33,
    URIL_NW_SERVICE_OPTION_TEMPORARILY_OUT_OF_ORDER = 34,
    URIL_NW_CALL_CANNOT_BE_IDENTIFIED = 38,
    URIL_NW_RETRY_UPON_ENTRY_INTO_A_NEW_CELL = 48,
    URIL_NW_SEMANTICALLY_INCORRECT_MESSAGE = 95,
    URIL_NW_INVALID_MANDATORY_INFORMATION = 96,
    URIL_NW_MESSAGE_TYPE_NON_EXISTENT_OR_NOT_IMPLEMENTED = 97,
    URIL_NW_MESSAGE_NOT_COMPATIBLE_WITH_PROTOCOL_STATE = 98,
    URIL_NW_INFORMATION_ELEMENT_NON_EXISTENT_OR_NOT_IMPLEMENTED = 99,
    URIL_NW_CONDITIONAL_IE_ERROR = 100,
    URIL_NW_PROTOCOL_ERROR_UNSPECIFIED = 111
} uril_nw_deny_reason_t_v01;

typedef struct {
    char long_eons[512 + 1];
    char short_eons[512 + 1];
    char mcc[3 + 1];
    char mnc[3 + 1];
} __attribute__((packed)) uril_nw_operator_name_t_v01;

typedef struct {
    uril_nw_operator_name_t_v01 operator_name;
    uril_nw_network_status_t_v01 network_status;
    uril_nw_rat_t_v01 rat;
} __attribute__((packed)) uril_nw_scan_entry_t_v01;

typedef struct {
    uril_nw_tech_t_v01 tech_domain;
    uril_nw_rat_t_v01 rat;
    uril_nw_roaming_state_t_v01 roaming;
    uint8_t denied;
    uril_nw_deny_reason_t_v01 deny_reason;
    uril_nw_service_t_v01 registration_state;
} __attribute__((packed)) uril_nw_registration_info_t_v01;

typedef struct {
    char mcc[3 + 1];
    char mnc[3 + 1];
    uint16_t lac;
    uint32_t cid;
} __attribute__((packed)) uril_cell_identity_gsm_t_v01;

typedef struct {
    char mcc[3 + 1];
    char mnc[3 + 1];
    uint32_t ci;
    uint16_t pci;
    uint16_t tac;
} __attribute__((packed)) uril_cell_identity_lte_t_v01;

typedef struct {
    char mcc[3 + 1];
    char mnc[3 + 1];
    uint16_t lac;
    uint32_t cid;
    uint16_t psc;
} __attribute__((packed)) uril_cell_identity_wcdma_t_v01;

typedef struct {
    char mcc[3 + 1];
    char mnc[3 + 1];
    uint16_t lac;
    uint32_t cid;
    uint8_t cpid;
} __attribute__((packed)) uril_cell_identity_tdscdma_t_v01;

typedef struct {
    uril_nw_selection_type_t_v01 nw_selection_type;
    char mcc[3 + 1];
    char mnc[3 + 1];
} __attribute__((packed)) uril_nw_selection_t_v01;

typedef struct {
    int8_t rssi;
} __attribute__((packed)) uril_nw_gsm_signal_strength_t_v01;

typedef struct {
    int8_t rssi;
    int16_t ecio;
} __attribute__((packed)) uril_nw_wcdma_signal_strength_t_v01;

typedef struct {
    int8_t rssi;
    int8_t rscp;
    int16_t ecio;
    int8_t sinr;
} __attribute__((packed)) uril_nw_tdscdma_signal_strength_t_v01;

typedef struct {
    int8_t rssi;
    int8_t rsrq;
    int8_t rsrp;
    int8_t snr;
} __attribute__((packed)) uril_nw_lte_signal_strength_t_v01;

typedef struct {
    uril_nw_pref_type_t_v01 preferred_nw_mode;
} __attribute__((packed)) uril_nw_set_pref_req_msg_v01;

typedef struct {
    uril_response_t_v01 response;
} __attribute__((packed)) uril_nw_set_pref_resp_msg_v01;

typedef struct {
    char __placeholder;
} __attribute__((packed)) uril_nw_get_pref_req_msg_v01;

typedef struct {
    uril_response_t_v01 response;
    uint8_t preferred_nw_mode_follows;
    uril_nw_pref_type_t_v01 preferred_nw_mode;
} __attribute__((packed)) uril_nw_get_pref_resp_msg_v01;

typedef struct {
    char __placeholder;
} __attribute__((packed)) uril_nw_get_registration_status_req_msg_v01;

typedef struct {
    uril_response_t_v01 response;
    uint8_t nw_cell_identity_gsm_follows;
    uril_cell_identity_gsm_t_v01 nw_cell_identity_gsm;
    uint8_t nw_cell_identity_lte_follows;
    uril_cell_identity_lte_t_v01 nw_cell_identity_lte;
    uint8_t nw_cell_identity_wcdma_follows;
    uril_cell_identity_wcdma_t_v01 nw_cell_identity_wcdma;
    uint8_t nw_cell_identity_tdscdma_follows;
    uril_cell_identity_tdscdma_t_v01 nw_cell_identity_tdscdma;
    uint8_t voice_registration_follows;
    uril_nw_registration_info_t_v01 voice_registration;
    uint8_t data_registration_follows;
    uril_nw_registration_info_t_v01 data_registration;
} __attribute__((packed)) uril_nw_get_registration_status_resp_msg_v01;

typedef struct {
    char __placeholder;
} __attribute__((packed)) uril_nw_scan_req_msg_v01;

typedef struct {
    uril_response_t_v01 response;
    uint8_t entries_follows;
    uint32_t entries_len;
    uril_nw_scan_entry_t_v01 entries[URIL_NW_SCAN_MAX_ENTRY_COUNT_V01];
} __attribute__((packed)) uril_nw_scan_resp_msg_v01;

typedef struct {
    uint8_t screen_state;
} __attribute__((packed)) uril_nw_set_screen_state_req_msg_v01;

typedef struct {
    uril_response_t_v01 response;
} __attribute__((packed)) uril_nw_set_screen_state_resp_msg_v01;

typedef struct {
    uril_nw_selection_t_v01 nw_selection_info;
} __attribute__((packed)) uril_nw_selection_req_msg_v01;

typedef struct {
    uril_response_t_v01 response;
} __attribute__((packed)) uril_nw_selection_resp_msg_v01;

typedef struct {
    char __placeholder;
} __attribute__((packed)) uril_nw_get_signal_strength_req_msg_v01;

typedef struct {
    uril_response_t_v01 response;
    uint8_t gsm_signal_strength_follows;
    uril_nw_gsm_signal_strength_t_v01 gsm_signal_strength;
    uint8_t wcdma_signal_strength_follows;
    uril_nw_wcdma_signal_strength_t_v01 wcdma_signal_strength;
    uint8_t tdscdma_signal_strength_follows;
    uril_nw_tdscdma_signal_strength_t_v01 tdscdma_signal_strength;
    uint8_t lte_signal_strength_follows;
    uril_nw_lte_signal_strength_t_v01 lte_signal_strength;
} __attribute__((packed)) uril_nw_get_signal_strength_resp_msg_v01;

typedef struct {
    char __placeholder;
} __attribute__((packed)) uril_nw_get_cell_access_state_req_msg_v01;

typedef struct {
    uril_response_t_v01 response;
    uint8_t nw_cell_access_state_follows;
    uril_nw_cell_access_state_t_v01 nw_cell_access_state;
} __attribute__((packed)) uril_nw_get_cell_access_state_resp_msg_v01;

#define URIL_NW_SET_PREF_REQ_V01 0x0500
#define URIL_NW_SET_PREF_RESP_V01 0x0500
#define URIL_NW_GET_PREF_REQ_V01 0x0501
#define URIL_NW_GET_PREF_RESP_V01 0x0501
#define URIL_NW_GET_REGISTRATION_STATUS_REQ_V01 0x0502
#define URIL_NW_GET_REGISTRATION_STATUS_RESP_V01 0x0502
#define URIL_NW_SCAN_REQ_V01 0x0503
#define URIL_NW_SCAN_RESP_V01 0x0503
#define URIL_NW_SET_SCREEN_REQ_V01 0x0504
#define URIL_NW_SET_SCREEN_RESP_V01 0x0504
#define URIL_NW_SELECTION_REQ_V01 0x0505
#define URIL_NW_SELECTION_RESP_V01 0x0505
#define URIL_NW_GET_SIGNAL_STRENGTH_REQ_V01 0x0506
#define URIL_NW_GET_SIGNAL_STRENGTH_RESP_V01 0x0506
#define URIL_NW_GET_CELL_ACCESS_STATE_REQ_V01 0x0507
#define URIL_NW_GET_CELL_ACCESS_STATE_RESP_V01 0x0507
#define URIL_NW_REGISTRATION_EVENT_IND_V01 0x0520
#define URIL_NW_SIGNAL_STRENGTH_EVENT_IND_V01 0x0521
#define URIL_NW_CELL_ACCESS_STATE_CHANGE_EVENT_IND_V01 0x0522

#ifdef __cplusplus
}
#endif
#endif /* URIL_NW_V01_H_ */
