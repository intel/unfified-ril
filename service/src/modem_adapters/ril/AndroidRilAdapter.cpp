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

#include <functional>
#include <thread>
#include <dlfcn.h>
#include <limits.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../../uril_svc_log.h"
#include "AndroidRilAdapter.h"
#include "IResponseProcessor.h"
#include "uril_common_v01.h"
#include "uril_nw_v01.h"

#ifndef RIL_SHLIB
#define RIL_SHLIB   1
#endif
#include <telephony/ril.h>

namespace uril {
namespace adapters {
namespace ril {

bool rilStrToInt(const char* rilStr, int* value) {
    if (rilStr == nullptr || *rilStr == '\0') {
        return false;
    }
    char* endPtr;
    errno = 0;
    long int val = strtoul(rilStr, &endPtr, 10);
    if (errno != 0 || *endPtr != '\0') {
        return false;
    }
    *value = val;
    return true;
}

RIL_PreferredNetworkType urilNwPrefToRilNwPref(
        uril_nw_pref_type_t_v01 preferred_nw_mode) {
    switch (preferred_nw_mode) {
    case uril_nw_pref_type_t_v01::URIL_NW_PREF_TYPE_GSM_WCDMA:
        return RIL_PreferredNetworkType::PREF_NET_TYPE_GSM_WCDMA;
    case uril_nw_pref_type_t_v01::URIL_NW_PREF_TYPE_GSM_ONLY:
        return RIL_PreferredNetworkType::PREF_NET_TYPE_GSM_ONLY;
    case uril_nw_pref_type_t_v01::URIL_NW_PREF_TYPE_WCDMA:
        return RIL_PreferredNetworkType::PREF_NET_TYPE_WCDMA;
    case uril_nw_pref_type_t_v01::URIL_NW_PREF_TYPE_GSM_WCDMA_AUTO:
        return RIL_PreferredNetworkType::PREF_NET_TYPE_GSM_WCDMA_AUTO;
    case uril_nw_pref_type_t_v01::URIL_NW_PREF_TYPE_CDMA_EVDO_AUTO:
        return RIL_PreferredNetworkType::PREF_NET_TYPE_CDMA_EVDO_AUTO;
    case uril_nw_pref_type_t_v01::URIL_NW_PREF_TYPE_CDMA_ONLY:
        return RIL_PreferredNetworkType::PREF_NET_TYPE_CDMA_ONLY;
    case uril_nw_pref_type_t_v01::URIL_NW_PREF_TYPE_EVDO_ONLY:
        return RIL_PreferredNetworkType::PREF_NET_TYPE_EVDO_ONLY;
    case uril_nw_pref_type_t_v01::URIL_NW_PREF_TYPE_GSM_WCDMA_CDMA_EVDO_AUTO:
        return RIL_PreferredNetworkType::PREF_NET_TYPE_GSM_WCDMA_CDMA_EVDO_AUTO;
    case uril_nw_pref_type_t_v01::URIL_NW_PREF_TYPE_LTE_CDMA_EVDO:
        return RIL_PreferredNetworkType::PREF_NET_TYPE_LTE_CDMA_EVDO;
    case uril_nw_pref_type_t_v01::URIL_NW_PREF_TYPE_LTE_GSM_WCDMA:
        return RIL_PreferredNetworkType::PREF_NET_TYPE_LTE_GSM_WCDMA;
    case uril_nw_pref_type_t_v01::URIL_NW_PREF_TYPE_LTE_CMDA_EVDO_GSM_WCDMA:
        return RIL_PreferredNetworkType::PREF_NET_TYPE_LTE_CMDA_EVDO_GSM_WCDMA;
    case uril_nw_pref_type_t_v01::URIL_NW_PREF_TYPE_LTE_ONLY:
        return RIL_PreferredNetworkType::PREF_NET_TYPE_LTE_ONLY;
    case uril_nw_pref_type_t_v01::URIL_NW_PREF_TYPE_LTE_WCDMA:
        return RIL_PreferredNetworkType::PREF_NET_TYPE_LTE_WCDMA;
    default:
        assert(0);
    }
    return RIL_PreferredNetworkType::PREF_NET_TYPE_LTE_GSM_WCDMA;
}

uril_nw_pref_type_t_v01 RilNwPrefToUrilNwPref(
        RIL_PreferredNetworkType preferred_nw_type) {
    switch (preferred_nw_type) {
    case RIL_PreferredNetworkType::PREF_NET_TYPE_GSM_WCDMA:
        return uril_nw_pref_type_t_v01::URIL_NW_PREF_TYPE_GSM_WCDMA;
    case RIL_PreferredNetworkType::PREF_NET_TYPE_GSM_ONLY:
        return uril_nw_pref_type_t_v01::URIL_NW_PREF_TYPE_GSM_ONLY;
    case RIL_PreferredNetworkType::PREF_NET_TYPE_WCDMA:
        return uril_nw_pref_type_t_v01::URIL_NW_PREF_TYPE_WCDMA;
    case RIL_PreferredNetworkType::PREF_NET_TYPE_GSM_WCDMA_AUTO:
        return uril_nw_pref_type_t_v01::URIL_NW_PREF_TYPE_GSM_WCDMA_AUTO;
    case RIL_PreferredNetworkType::PREF_NET_TYPE_CDMA_EVDO_AUTO:
        return uril_nw_pref_type_t_v01::URIL_NW_PREF_TYPE_CDMA_EVDO_AUTO;
    case RIL_PreferredNetworkType::PREF_NET_TYPE_CDMA_ONLY:
        return uril_nw_pref_type_t_v01::URIL_NW_PREF_TYPE_CDMA_ONLY;
    case RIL_PreferredNetworkType::PREF_NET_TYPE_EVDO_ONLY:
        return uril_nw_pref_type_t_v01::URIL_NW_PREF_TYPE_EVDO_ONLY;
    case RIL_PreferredNetworkType::PREF_NET_TYPE_GSM_WCDMA_CDMA_EVDO_AUTO:
        return uril_nw_pref_type_t_v01::URIL_NW_PREF_TYPE_GSM_WCDMA_CDMA_EVDO_AUTO;
    case RIL_PreferredNetworkType::PREF_NET_TYPE_LTE_CDMA_EVDO:
        return uril_nw_pref_type_t_v01::URIL_NW_PREF_TYPE_LTE_CDMA_EVDO;
    case RIL_PreferredNetworkType::PREF_NET_TYPE_LTE_GSM_WCDMA:
        return uril_nw_pref_type_t_v01::URIL_NW_PREF_TYPE_LTE_GSM_WCDMA;
    case RIL_PreferredNetworkType::PREF_NET_TYPE_LTE_CMDA_EVDO_GSM_WCDMA:
        return uril_nw_pref_type_t_v01::URIL_NW_PREF_TYPE_LTE_CMDA_EVDO_GSM_WCDMA;
    case RIL_PreferredNetworkType::PREF_NET_TYPE_LTE_ONLY:
        return uril_nw_pref_type_t_v01::URIL_NW_PREF_TYPE_LTE_ONLY;
    case RIL_PreferredNetworkType::PREF_NET_TYPE_LTE_WCDMA:
        return uril_nw_pref_type_t_v01::URIL_NW_PREF_TYPE_LTE_WCDMA;
    default:
        assert(0);
    }
    return uril_nw_pref_type_t_v01::URIL_NW_PREF_TYPE_LTE_GSM_WCDMA;
}

uril_nw_rat_t_v01 rilRatToUrilRat(RIL_RadioTechnology radio_tech) {
    switch (radio_tech) {
    case RIL_RadioTechnology::RADIO_TECH_UNKNOWN:
        return uril_nw_rat_t_v01::URIL_NW_RAT_NONE_V01;
    case RIL_RadioTechnology::RADIO_TECH_GPRS:
        return uril_nw_rat_t_v01::URIL_NW_RAT_GPRS_V01;
    case RIL_RadioTechnology::RADIO_TECH_EDGE:
        return uril_nw_rat_t_v01::URIL_NW_RAT_EDGE_V01;
    case RIL_RadioTechnology::RADIO_TECH_UMTS:
        return uril_nw_rat_t_v01::URIL_NW_RAT_UMTS_V01;
    case RIL_RadioTechnology::RADIO_TECH_IS95A:
        return uril_nw_rat_t_v01::URIL_NW_RAT_IS95A_V01;
    case RIL_RadioTechnology::RADIO_TECH_IS95B:
        return uril_nw_rat_t_v01::URIL_NW_RAT_IS95B_V01;
    case RIL_RadioTechnology::RADIO_TECH_1xRTT:
        return uril_nw_rat_t_v01::URIL_NW_RAT_1xRTT_V01;
    case RIL_RadioTechnology::RADIO_TECH_EVDO_0:
        return uril_nw_rat_t_v01::URIL_NW_RAT_EVDO_0_V01;
    case RIL_RadioTechnology::RADIO_TECH_EVDO_A:
        return uril_nw_rat_t_v01::URIL_NW_RAT_EVDO_A_V01;
    case RIL_RadioTechnology::RADIO_TECH_HSDPA:
        return uril_nw_rat_t_v01::URIL_NW_RAT_HSDPA_V01;
    case RIL_RadioTechnology::RADIO_TECH_HSUPA:
        return uril_nw_rat_t_v01::URIL_NW_RAT_HSUPA_V01;
    case RIL_RadioTechnology::RADIO_TECH_HSPA:
        return uril_nw_rat_t_v01::URIL_NW_RAT_HSPA_V01;
    case RIL_RadioTechnology::RADIO_TECH_EVDO_B:
        return uril_nw_rat_t_v01::URIL_NW_RAT_EVDO_B_V01;
    case RIL_RadioTechnology::RADIO_TECH_EHRPD:
        return uril_nw_rat_t_v01::URIL_NW_RAT_EHRPD_V01;
    case RIL_RadioTechnology::RADIO_TECH_LTE:
        return uril_nw_rat_t_v01::URIL_NW_RAT_LTE_V01;
    case RIL_RadioTechnology::RADIO_TECH_HSPAP:
        return uril_nw_rat_t_v01::URIL_NW_RAT_HSPAP_V01;
    case RIL_RadioTechnology::RADIO_TECH_GSM:
        return uril_nw_rat_t_v01::URIL_NW_RAT_GSM_V01;
    case RIL_RadioTechnology::RADIO_TECH_TD_SCDMA:
        return uril_nw_rat_t_v01::URIL_NW_RAT_TD_SCDMA_V01;
    case RIL_RadioTechnology::RADIO_TECH_IWLAN:
         return uril_nw_rat_t_v01::URIL_NW_RAT_IWLAN_V01;
    default:
        assert(0);
    }
    return uril_nw_rat_t_v01::URIL_NW_RAT_NONE_V01;
}

RIL_RadioTechnology urilDataBearerTechTypeToRilRadioTech(
        uril_data_bearer_tech_info_t_v01 data_bearer_tech) {
    switch (data_bearer_tech) {
    case uril_data_bearer_tech_info_t_v01::URIL_DATA_BEARER_TECH_TYPE_UNKNOWN_V01:
        return RIL_RadioTechnology::RADIO_TECH_UNKNOWN;
    case uril_data_bearer_tech_info_t_v01::URIL_DATA_BEARER_TECH_TYPE_CDMA_1X_V01:
        return RIL_RadioTechnology::RADIO_TECH_1xRTT;
    case uril_data_bearer_tech_info_t_v01::URIL_DATA_BEARER_TECH_TYPE_EVDO_REV0_V01:
        return RIL_RadioTechnology::RADIO_TECH_EVDO_0;
    case uril_data_bearer_tech_info_t_v01::URIL_DATA_BEARER_TECH_TYPE_EVDO_REVA_V01:
        return RIL_RadioTechnology::RADIO_TECH_EVDO_A;
    case uril_data_bearer_tech_info_t_v01::URIL_DATA_BEARER_TECH_TYPE_EVDO_REVB_V01:
        return RIL_RadioTechnology::RADIO_TECH_EVDO_B;
    case uril_data_bearer_tech_info_t_v01::URIL_DATA_BEARER_TECH_TYPE_EHRPD_V01:
        return RIL_RadioTechnology::RADIO_TECH_EHRPD;
    case uril_data_bearer_tech_info_t_v01::URIL_DATA_BEARER_TECH_TYPE_FMC_V01:
        // TODO
        return RIL_RadioTechnology::RADIO_TECH_UNKNOWN;
    case uril_data_bearer_tech_info_t_v01::URIL_DATA_BEARER_TECH_TYPE_HRPD_V01:
        // TODO
        return RIL_RadioTechnology::RADIO_TECH_UNKNOWN;
    case uril_data_bearer_tech_info_t_v01::URIL_DATA_BEARER_TECH_TYPE_3GPP2_WLAN_V01:
        // TODO
        return RIL_RadioTechnology::RADIO_TECH_UNKNOWN;
    case uril_data_bearer_tech_info_t_v01::URIL_DATA_BEARER_TECH_TYPE_WCDMA_V01:
        return RIL_RadioTechnology::RADIO_TECH_UMTS;
    case uril_data_bearer_tech_info_t_v01::URIL_DATA_BEARER_TECH_TYPE_GPRS_V01:
        return RIL_RadioTechnology::RADIO_TECH_GPRS;
    case uril_data_bearer_tech_info_t_v01::URIL_DATA_BEARER_TECH_TYPE_HSDPA_V01:
        return RIL_RadioTechnology::RADIO_TECH_HSDPA;
    case uril_data_bearer_tech_info_t_v01::URIL_DATA_BEARER_TECH_TYPE_HSUPA_V01:
        return RIL_RadioTechnology::RADIO_TECH_HSUPA;
    case uril_data_bearer_tech_info_t_v01::URIL_DATA_BEARER_TECH_TYPE_EDGE_V01:
        return RIL_RadioTechnology::RADIO_TECH_EDGE;
    case uril_data_bearer_tech_info_t_v01::URIL_DATA_BEARER_TECH_TYPE_LTE_V01:
        return RIL_RadioTechnology::RADIO_TECH_LTE;
    case uril_data_bearer_tech_info_t_v01::URIL_DATA_BEARER_TECH_TYPE_HSDPA_PLUS_V01:
        return RIL_RadioTechnology::RADIO_TECH_HSPAP;
    case uril_data_bearer_tech_info_t_v01::URIL_DATA_BEARER_TECH_TYPE_DC_HSDPA_PLUS_V01:
        // TODO
        return RIL_RadioTechnology::RADIO_TECH_HSPAP;
    case uril_data_bearer_tech_info_t_v01::URIL_DATA_BEARER_TECH_TYPE_HSPA_V01:
        return RIL_RadioTechnology::RADIO_TECH_HSPA;
    case uril_data_bearer_tech_info_t_v01::URIL_DATA_BEARER_TECH_TYPE_64_QAM_V01:
        // TODO
        return RIL_RadioTechnology::RADIO_TECH_HSPAP;
    case uril_data_bearer_tech_info_t_v01::URIL_DATA_BEARER_TECH_TYPE_TDSCDMA_V01:
        return RIL_RadioTechnology::RADIO_TECH_TD_SCDMA;
    case uril_data_bearer_tech_info_t_v01::URIL_DATA_BEARER_TECH_TYPE_GSM_V01:
        return RIL_RadioTechnology::RADIO_TECH_GSM;
    case uril_data_bearer_tech_info_t_v01::URIL_DATA_BEARER_TECH_TYPE_3GPP_WLAN_V01:
        return RIL_RadioTechnology::RADIO_TECH_IWLAN;
    default:
        assert(0);
    }
    return RIL_RadioTechnology::RADIO_TECH_UNKNOWN;
}

uril_data_call_failure_cause_t_v01 rilDataCallFailCauseToUrilDataFailCause(
        RIL_DataCallFailCause data_fail_cause) {
    switch (data_fail_cause) {
    case RIL_DataCallFailCause::PDP_FAIL_NONE:
        return uril_data_call_failure_cause_t_v01::URIL_DATA_FAIL_CAUSE_NONE;
    case RIL_DataCallFailCause::PDP_FAIL_OPERATOR_BARRED:
        return uril_data_call_failure_cause_t_v01::URIL_DATA_FAIL_CAUSE_OPERATOR_BARRED;
    case RIL_DataCallFailCause::PDP_FAIL_INSUFFICIENT_RESOURCES:
        return uril_data_call_failure_cause_t_v01::URIL_DATA_FAIL_CAUSE_INSUFFICIENT_RESOURCES;
    case RIL_DataCallFailCause::PDP_FAIL_MISSING_UKNOWN_APN:
        return uril_data_call_failure_cause_t_v01::URIL_DATA_FAIL_CAUSE_MISSING_UKNOWN_APN;
    case RIL_DataCallFailCause::PDP_FAIL_UNKNOWN_PDP_ADDRESS_TYPE:
        return uril_data_call_failure_cause_t_v01::URIL_DATA_FAIL_CAUSE_UNKNOWN_PDP_ADDRESS_TYPE;
    case RIL_DataCallFailCause::PDP_FAIL_USER_AUTHENTICATION:
        return uril_data_call_failure_cause_t_v01::URIL_DATA_FAIL_CAUSE_USER_AUTHENTICATION;
    case RIL_DataCallFailCause::PDP_FAIL_ACTIVATION_REJECT_GGSN:
        return uril_data_call_failure_cause_t_v01::URIL_DATA_FAIL_CAUSE_ACTIVATION_REJECT_GGSN;
    case RIL_DataCallFailCause::PDP_FAIL_ACTIVATION_REJECT_UNSPECIFIED:
        return uril_data_call_failure_cause_t_v01::URIL_DATA_FAIL_CAUSE_ACTIVATION_REJECT_UNSPECIFIED;
    case RIL_DataCallFailCause::PDP_FAIL_SERVICE_OPTION_NOT_SUPPORTED:
        return uril_data_call_failure_cause_t_v01::URIL_DATA_FAIL_CAUSE_SERVICE_OPTION_NOT_SUPPORTED;
    case RIL_DataCallFailCause::PDP_FAIL_SERVICE_OPTION_NOT_SUBSCRIBED:
        return uril_data_call_failure_cause_t_v01::URIL_DATA_FAIL_CAUSE_SERVICE_OPTION_NOT_SUBSCRIBED;
    case RIL_DataCallFailCause::PDP_FAIL_SERVICE_OPTION_OUT_OF_ORDER:
        return uril_data_call_failure_cause_t_v01::URIL_DATA_FAIL_CAUSE_SERVICE_OPTION_OUT_OF_ORDER;
    case RIL_DataCallFailCause::PDP_FAIL_NSAPI_IN_USE:
        return uril_data_call_failure_cause_t_v01::URIL_DATA_FAIL_CAUSE_NSAPI_IN_USE;
    case RIL_DataCallFailCause::PDP_FAIL_REGULAR_DEACTIVATION:
        return uril_data_call_failure_cause_t_v01::URIL_DATA_FAIL_CAUSE_REGULAR_DEACTIVATION;
    case RIL_DataCallFailCause::PDP_FAIL_ONLY_IPV4_ALLOWED:
        return uril_data_call_failure_cause_t_v01::URIL_DATA_FAIL_CAUSE_ONLY_IPV4_ALLOWED;
    case RIL_DataCallFailCause::PDP_FAIL_ONLY_IPV6_ALLOWED:
        return uril_data_call_failure_cause_t_v01::URIL_DATA_FAIL_CAUSE_ONLY_IPV6_ALLOWED;
    case RIL_DataCallFailCause::PDP_FAIL_ONLY_SINGLE_BEARER_ALLOWED:
        return uril_data_call_failure_cause_t_v01::URIL_DATA_FAIL_CAUSE_ONLY_SINGLE_BEARER_ALLOWED;
    case RIL_DataCallFailCause::PDP_FAIL_PROTOCOL_ERRORS:
        return uril_data_call_failure_cause_t_v01::URIL_DATA_FAIL_CAUSE_PROTOCOL_ERRORS;
    case RIL_DataCallFailCause::PDP_FAIL_VOICE_REGISTRATION_FAIL:
        return uril_data_call_failure_cause_t_v01::URIL_DATA_FAIL_CAUSE_VOICE_REGISTRATION_FAIL;
    case RIL_DataCallFailCause::PDP_FAIL_DATA_REGISTRATION_FAIL:
        return uril_data_call_failure_cause_t_v01::URIL_DATA_FAIL_CAUSE_DATA_REGISTRATION_FAIL;
    case RIL_DataCallFailCause::PDP_FAIL_SIGNAL_LOST:
        return uril_data_call_failure_cause_t_v01::URIL_DATA_FAIL_CAUSE_SIGNAL_LOST;
    case RIL_DataCallFailCause::PDP_FAIL_PREF_RADIO_TECH_CHANGED:
        return uril_data_call_failure_cause_t_v01::URIL_DATA_FAIL_CAUSE_PREF_RADIO_TECH_CHANGED;
    case RIL_DataCallFailCause::PDP_FAIL_RADIO_POWER_OFF:
        return uril_data_call_failure_cause_t_v01::URIL_DATA_FAIL_CAUSE_RADIO_POWER_OFF;
    case RIL_DataCallFailCause::PDP_FAIL_TETHERED_CALL_ACTIVE:
        return uril_data_call_failure_cause_t_v01::URIL_DATA_FAIL_CAUSE_TETHERED_CALL_ACTIVE;
    case RIL_DataCallFailCause::PDP_FAIL_ERROR_UNSPECIFIED:
        return uril_data_call_failure_cause_t_v01::URIL_DATA_FAIL_CAUSE_ERROR_UNSPECIFIED;
    default:
        // Handle 3GPP causes not defined in enum ?
        // TODO check this :)
        return (uril_data_call_failure_cause_t_v01) data_fail_cause;
    }
    return uril_data_call_failure_cause_t_v01::URIL_DATA_FAIL_CAUSE_ERROR_UNSPECIFIED;
}

bool rilIpAddrToUrilDataAddr(const char *addr,
        uril_data_addr_t_v01 *iril_addr, const char **endAddr) {
    if (addr == nullptr) {
        iril_addr->addr[0] = '\0';
        iril_addr->prefix_len = 0;
        return false;
    }

    while (*addr == ' ')
        addr += 1;
    char *end = (char*) strpbrk(addr, " /");
    size_t len;
    if (end == nullptr)
        len = strlen(addr);
    else
        len = end - addr;
    if (len == 0)
        return false;
    assert(len < sizeof(iril_addr->addr));
    memcpy(iril_addr->addr, addr, len);
    iril_addr->addr[len] = '\0';

    if (end == nullptr || *end != '/') {
        if (endAddr) {
            if (end == nullptr)
                *endAddr = addr + len;
            else
                *endAddr = end + 1;
        }
        iril_addr->prefix_len = strchr(iril_addr->addr, ':') != nullptr ? 128 : 32;
    } else {
        iril_addr->prefix_len = strtoul(end + 1, (char **) endAddr, 10);
    }
    return true;
}

int urilAuthTypeToRilAuthType(uril_data_auth_type_t_v01 auth_type) {
    switch (auth_type) {
    case uril_data_auth_type_t_v01::URIL_DATA_AUTH_TYPE_NONE:
        return 0;
    case uril_data_auth_type_t_v01::URIL_DATA_AUTH_TYPE_PAP:
        return 1;
    case uril_data_auth_type_t_v01::URIL_DATA_AUTH_TYPE_CHAP:
        return 2;
    case uril_data_auth_type_t_v01::URIL_DATA_AUTH_TYPE_PAPCHAP:
        return 3;
    default:
        assert(0);
    }
    return 0;
}

const char *urilPdpTypeToRilPdpType(
        uril_data_pdp_type_t_v01 pdp_type) {
    switch (pdp_type) {
    case uril_data_pdp_type_t_v01::URIL_DATA_PDP_TYPE_NONE:
        return "";
    case uril_data_pdp_type_t_v01::URIL_DATA_PDP_TYPE_IP:
        return "IP";
    case uril_data_pdp_type_t_v01::URIL_DATA_PDP_TYPE_IPV6:
        return "IPV6";
    case uril_data_pdp_type_t_v01::URIL_DATA_PDP_TYPE_IPV4V6:
        return "IPV4V6";
    case uril_data_pdp_type_t_v01::URIL_DATA_PDP_TYPE_PPP:
        return "PPP";
    default:
        assert(0);
    }
    return "";
}

void rilDataCallResponseToIRilDataCallInfo(
        uril_data_call_info_t_v01 *iril_call,
        const RIL_Data_Call_Response_v11 *ril_call) {
    iril_call->failure_cause = rilDataCallFailCauseToUrilDataFailCause(
            (RIL_DataCallFailCause) ril_call->status);
    iril_call->suggested_retry_time = ril_call->suggestedRetryTime;
    iril_call->cid = ril_call->cid;

    switch (ril_call->active) {
    case 0:
        iril_call->phy_link_state = URIL_DATA_PHYSLINK_STATE_INACTIVE_V01;
        break;
    case 1:
        iril_call->phy_link_state =
                URIL_DATA_PHYSLINK_STATE_ACTIVE_DOWN_V01;
        break;
    case 2:
        iril_call->phy_link_state = URIL_DATA_PHYSLINK_STATE_ACTIVE_UP_V01;
        break;
    default:
        assert(0);
    }

    if (ril_call->type == nullptr)
        iril_call->pdp_type = URIL_DATA_PDP_TYPE_NONE;
    else if (!strcmp(ril_call->type, "IP"))
        iril_call->pdp_type = URIL_DATA_PDP_TYPE_IP;
    else if (!strcmp(ril_call->type, "IPV6"))
        iril_call->pdp_type = URIL_DATA_PDP_TYPE_IPV6;
    else if (!strcmp(ril_call->type, "IPV4V6"))
        iril_call->pdp_type = URIL_DATA_PDP_TYPE_IPV4V6;
    else if (!strcmp(ril_call->type, "PPP"))
        iril_call->pdp_type = URIL_DATA_PDP_TYPE_PPP;
    else
        iril_call->pdp_type = URIL_DATA_PDP_TYPE_NONE;

    if (ril_call->ifname == nullptr)
        iril_call->interface_name[0] = '\0';
    else
        snprintf(iril_call->interface_name, sizeof(iril_call->interface_name),
                "%s", ril_call->ifname);

    iril_call->pdp_addrs_len = 0;
    const char *addr = ril_call->addresses;
    if (addr) {
        while (rilIpAddrToUrilDataAddr(addr,
                &iril_call->pdp_addrs[iril_call->pdp_addrs_len], &addr)
                && iril_call->pdp_addrs_len < URIL_DATA_MAX_ADDR_COUNT_V01) {
            iril_call->pdp_addrs_len += 1;
        }
    }

    rilIpAddrToUrilDataAddr(ril_call->gateways, &iril_call->gtwy_addr,
            nullptr);

    addr = ril_call->dnses;
    if (rilIpAddrToUrilDataAddr(addr, &iril_call->dnsp_addr, &addr))
        rilIpAddrToUrilDataAddr(addr, &iril_call->dnss_addr, nullptr);

    rilIpAddrToUrilDataAddr(ril_call->pcscf, &iril_call->pcscf_addr, nullptr);

    iril_call->mtu = ril_call->mtu;
}

uril_nw_deny_reason_t_v01 rilDenyReasonToUrilDenyReason(
        int denyReason) {
    /*
     * 0 - General
     *                                 1 - Authentication Failure
     *                                 2 - IMSI unknown in HLR
     *                                 3 - Illegal MS
     *                                 4 - Illegal ME
     *                                 5 - PLMN not allowed
     *                                 6 - Location area not allowed
     *                                 7 - Roaming not allowed
     *                                 8 - No Suitable Cells in this Location Area
     *                                 9 - Network failure
     *                                10 - Persistent location update reject
     *                                11 - PLMN not allowed
     *                                12 - Location area not allowed
     *                                13 - Roaming not allowed in this Location Area
     *                                15 - No Suitable Cells in this Location Area
     *                                17 - Network Failure
     *                                20 - MAC Failure
     *                                21 - Sync Failure
     *                                22 - Congestion
     *                                23 - GSM Authentication unacceptable
     *                                25 - Not Authorized for this CSG
     *                                32 - Service option not supported
     *                                33 - Requested service option not subscribed
     *                                34 - Service option temporarily out of order
     *                                38 - Call cannot be identified
     *                                48-63 - Retry upon entry into a new cell
     *                                95 - Semantically incorrect message
     *                                96 - Invalid mandatory information
     *                                97 - Message type non-existent or not implemented
     *                                98 - Message not compatible with protocol state
     *                                99 - Information element non-existent or not implemented
     *                               100 - Conditional IE error
     *                               101 - Message not compatible with protocol state
     *                               111 - Protocol error, unspecified
     */
    return uril_nw_deny_reason_t_v01::URIL_NW_GENERAL;
}

bool rilNwRegToUrilNwReg(uril_nw_registration_info_t_v01* regInfo,
        const char* regstate, const char* radiotech, const char* denyreason) {
    int reg_state;
    RIL_RadioTechnology radio_tech;
    uril_nw_deny_reason_t_v01 deny_reason;
    if (rilStrToInt(regstate, &reg_state)
            && rilStrToInt(radiotech, (int*) &radio_tech)) {
        regInfo->tech_domain = URIL_NW_TECH_NONE_V01;

        /* ril.h:
         *              0 - Not registered, MT is not currently searching
         *                  a new operator to register
         *              1 - Registered, home network
         *              2 - Not registered, but MT is currently searching
         *                  a new operator to register
         *              3 - Registration denied
         *              4 - Unknown
         *              5 - Registered, roaming
         *             10 - Same as 0, but indicates that emergency calls
         *                  are enabled.
         *             12 - Same as 2, but indicates that emergency calls
         *                  are enabled.
         *             13 - Same as 3, but indicates that emergency calls
         *                  are enabled.
         *             14 - Same as 4, but indicates that emergency calls
         *                  are enabled.
         */

        switch (reg_state) {
        case 0:
        case 10:
        case 2:
        case 12:
        case 4:
        case 14:
            regInfo->roaming = URIL_NW_ROAMING_STATE_OFF_V01;
            regInfo->registration_state =
                    reg_state < 10 ?
                            URIL_NW_SERVICE_NONE_V01 :
                            URIL_NW_SERVICE_LIMITED_V01;
            regInfo->denied = 0;
            break;

        case 3:
        case 13:
            regInfo->roaming = URIL_NW_ROAMING_STATE_OFF_V01;
            regInfo->registration_state =
                    reg_state < 10 ?
                            URIL_NW_SERVICE_NONE_V01 :
                            URIL_NW_SERVICE_LIMITED_V01;
            regInfo->denied = 1;
            if (rilStrToInt(denyreason, (int*) &deny_reason)) {
                deny_reason =
                        uril_nw_deny_reason_t_v01::URIL_NW_GENERAL;
            }
            regInfo->deny_reason = deny_reason;
            break;

        case 1:
        case 5:
            regInfo->roaming =
                    reg_state == 1 ?
                            URIL_NW_ROAMING_STATE_OFF_V01 :
                            URIL_NW_ROAMING_STATE_ON_V01;
            regInfo->registration_state = URIL_NW_SERVICE_FULL_V01;
            regInfo->denied = 0;
            break;

        default:
            assert(0);
            break;
        }

        regInfo->rat = rilRatToUrilRat(radio_tech);
        return true;
    } else {
        return false;
    }
}

bool rilCellToUrilCellVoice(
        uril_nw_get_registration_status_resp_msg_v01* reg_status,
        const char *ril_radiotech, const char *ril_lac, const char *ril_cid,
        const char *ril_psc) {
    int rat;
    if (!rilStrToInt(ril_radiotech, &rat)) {
        return false;
    }
    int lac = -1;
    rilStrToInt(ril_lac, &lac);
    int cid = -1;
    rilStrToInt(ril_cid, &cid);
    int psc = -1;
    rilStrToInt(ril_psc, &psc);

    switch (rat) {
    case RIL_RadioTechnology::RADIO_TECH_GPRS:
    case RIL_RadioTechnology::RADIO_TECH_EDGE:
    case RIL_RadioTechnology::RADIO_TECH_GSM:
        reg_status->nw_cell_identity_gsm_follows = 1;
        reg_status->nw_cell_identity_gsm.lac = lac;
        reg_status->nw_cell_identity_gsm.cid = cid;
        break;
    case RIL_RadioTechnology::RADIO_TECH_UMTS:
    case RIL_RadioTechnology::RADIO_TECH_HSDPA:
    case RIL_RadioTechnology::RADIO_TECH_HSUPA:
    case RIL_RadioTechnology::RADIO_TECH_HSPA:
    case RIL_RadioTechnology::RADIO_TECH_HSPAP:
        reg_status->nw_cell_identity_wcdma_follows = 1;
        reg_status->nw_cell_identity_wcdma.lac = lac;
        reg_status->nw_cell_identity_wcdma.cid = cid;
        reg_status->nw_cell_identity_wcdma.psc = psc;
        break;
    case RIL_RadioTechnology::RADIO_TECH_TD_SCDMA:
        reg_status->nw_cell_identity_tdscdma_follows = 1;
        reg_status->nw_cell_identity_tdscdma.lac = lac;
        reg_status->nw_cell_identity_tdscdma.cid = cid;
        /** TODO:
         *    uril_nw_get_registration_status_resp_msg_v01.uril_cell_identity_tdscdma_t_v01.cpid
         */
        break;
    case RIL_RadioTechnology::RADIO_TECH_LTE:
        /* Will be filled in data reply */
        break;
    }
    return true;
}

bool rilCellToUrilCellData(
        uril_nw_get_registration_status_resp_msg_v01* reg_status,
        const char* ril_radiotech, const char* ril_ltetac,
        const char* ril_ltecid, const char* ril_lteeci) {
    int rat;
    if (!rilStrToInt(ril_radiotech, &rat)) {
        return false;
    }
    int tac = -1;
    rilStrToInt(ril_ltetac, &tac);
    int cid = -1;
    rilStrToInt(ril_ltecid, &cid);
    int eci = -1;
    rilStrToInt(ril_lteeci, &eci);
    switch (rat) {
    case RIL_RadioTechnology::RADIO_TECH_LTE:
        reg_status->nw_cell_identity_lte_follows = 1;
        reg_status->nw_cell_identity_lte.tac = tac;
        reg_status->nw_cell_identity_lte.ci = eci;
        reg_status->nw_cell_identity_lte.pci = cid;
        break;
    }
    return true;
}

bool rilOperatorNameToUrilOperatorName(
        uril_nw_operator_name_t_v01* operator_name,
        const char* ril_long_name, const char* ril_short_name,
        const char *ril_mncmcc) {
    if (ril_long_name != nullptr && ril_short_name != nullptr
            && ril_mncmcc != nullptr) {
        snprintf(operator_name->long_eons, sizeof(operator_name->long_eons),
                "%s", ril_long_name);
        snprintf(operator_name->short_eons, sizeof(operator_name->short_eons),
                "%s", ril_short_name);
        strncpy(operator_name->mcc, ril_mncmcc, sizeof(operator_name->mcc) - 1);
        strncpy(operator_name->mnc, ril_mncmcc + 3,
                sizeof(operator_name->mnc) - 1);
        return true;
    }
    return false;
}

void rilErrnoToUrilResponse(RIL_Errno e, uril_response_t_v01* rsp) {
    if (e == RIL_Errno::RIL_E_SUCCESS) {
        rsp->result = URIL_RESULT_SUCCESS_V01;
        rsp->error = URIL_ERROR_SUCCESS_V01;
    } else {
        rsp->result = URIL_RESULT_FAILURE_V01;
        switch (e) {
        case RIL_Errno::RIL_E_RADIO_NOT_AVAILABLE:
            rsp->error =
                    uril_error_t_v01::URIL_ERROR_RADIO_NOT_AVAILABLE_V01;
            break;
        case RIL_Errno::RIL_E_PASSWORD_INCORRECT:
            rsp->error =
                    uril_error_t_v01::URIL_ERROR_PASSWORD_INCORRECT_V01;
            break;
        case RIL_Errno::RIL_E_SIM_PIN2:
            rsp->error = uril_error_t_v01::URIL_ERROR_SIM_PIN2_V01;
            break;
        case RIL_Errno::RIL_E_SIM_PUK2:
            rsp->error = uril_error_t_v01::URIL_ERROR_SIM_PUK2_V01;
            break;
        case RIL_Errno::RIL_E_REQUEST_NOT_SUPPORTED:
            rsp->error =
                    uril_error_t_v01::URIL_ERROR_REQUEST_NOT_SUPPORTED_V01;
            break;
        case RIL_Errno::RIL_E_CANCELLED:
            rsp->error = uril_error_t_v01::URIL_ERROR_CANCELLED_V01;
            break;
        case RIL_Errno::RIL_E_OP_NOT_ALLOWED_DURING_VOICE_CALL:
            rsp->error =
                    uril_error_t_v01::URIL_ERROR_OP_NOT_ALLOWED_DURING_VOICE_CALL_V01;
            break;
        case RIL_Errno::RIL_E_OP_NOT_ALLOWED_BEFORE_REG_TO_NW:
            rsp->error =
                    uril_error_t_v01::URIL_ERROR_OP_NOT_ALLOWED_BEFORE_REG_TO_NW_V01;
            break;
        case RIL_Errno::RIL_E_SMS_SEND_FAIL_RETRY:
            rsp->error =
                    uril_error_t_v01::URIL_ERROR_SMS_SEND_FAIL_RETRY_V01;
            break;
        case RIL_Errno::RIL_E_SIM_ABSENT:
            rsp->error = uril_error_t_v01::URIL_ERROR_SIM_ABSENT_V01;
            break;
        case RIL_Errno::RIL_E_SUBSCRIPTION_NOT_AVAILABLE:
            rsp->error =
                    uril_error_t_v01::URIL_ERROR_SUBSCRIPTION_NOT_AVAILABLE_V01;
            break;
        case RIL_Errno::RIL_E_MODE_NOT_SUPPORTED:
            rsp->error =
                    uril_error_t_v01::URIL_ERROR_MODE_NOT_SUPPORTED_V01;
            break;
        case RIL_Errno::RIL_E_FDN_CHECK_FAILURE:
            rsp->error =
                    uril_error_t_v01::URIL_ERROR_FDN_CHECK_FAILURE_V01;
            break;
        case RIL_Errno::RIL_E_ILLEGAL_SIM_OR_ME:
            rsp->error =
                    uril_error_t_v01::URIL_ERROR_ILLEGAL_SIM_OR_ME_V01;
            break;
        case RIL_Errno::RIL_E_MISSING_RESOURCE:
            rsp->error =
                    uril_error_t_v01::URIL_ERROR_MISSING_RESOURCE_V01;
            break;
        case RIL_Errno::RIL_E_NO_SUCH_ELEMENT:
            rsp->error =
                    uril_error_t_v01::URIL_ERROR_NO_SUCH_ELEMENT_V01;
            break;
        case RIL_Errno::RIL_E_DIAL_MODIFIED_TO_USSD:
            rsp->error =
                    uril_error_t_v01::URIL_ERROR_DIAL_MODIFIED_TO_USSD_V01;
            break;
        case RIL_Errno::RIL_E_DIAL_MODIFIED_TO_SS:
            rsp->error =
                    uril_error_t_v01::URIL_ERROR_DIAL_MODIFIED_TO_SS_V01;
            break;
        case RIL_Errno::RIL_E_DIAL_MODIFIED_TO_DIAL:
            rsp->error =
                    uril_error_t_v01::URIL_ERROR_DIAL_MODIFIED_TO_DIAL_V01;
            break;
        case RIL_Errno::RIL_E_USSD_MODIFIED_TO_DIAL:
            rsp->error =
                    uril_error_t_v01::URIL_ERROR_USSD_MODIFIED_TO_DIAL_V01;
            break;
        case RIL_Errno::RIL_E_USSD_MODIFIED_TO_SS:
            rsp->error =
                    uril_error_t_v01::URIL_ERROR_USSD_MODIFIED_TO_SS_V01;
            break;
        case RIL_Errno::RIL_E_USSD_MODIFIED_TO_USSD:
            rsp->error =
                    uril_error_t_v01::URIL_ERROR_USSD_MODIFIED_TO_USSD_V01;
            break;
        case RIL_Errno::RIL_E_SS_MODIFIED_TO_DIAL:
            rsp->error =
                    uril_error_t_v01::URIL_ERROR_SS_MODIFIED_TO_DIAL_V01;
            break;
        case RIL_Errno::RIL_E_SS_MODIFIED_TO_USSD:
            rsp->error =
                    uril_error_t_v01::URIL_ERROR_SS_MODIFIED_TO_USSD_V01;
            break;
        case RIL_Errno::RIL_E_SUBSCRIPTION_NOT_SUPPORTED:
            rsp->error =
                    uril_error_t_v01::URIL_ERROR_SUBSCRIPTION_NOT_SUPPORTED_V01;
            break;
        case RIL_Errno::RIL_E_SS_MODIFIED_TO_SS:
            rsp->error =
                    uril_error_t_v01::URIL_ERROR_SS_MODIFIED_TO_SS_V01;
            break;
        default:
            rsp->error =
                    uril_error_t_v01::URIL_ERROR_GENERIC_FAILURE_V01;
            break;
            /*case RIL_Errno::URIL_ERROR_LCE_NOT_SUPPORTED_V01:
             rsp->error =
             uril_error_t_v01::URIL_ERROR_LCE_NOT_SUPPORTED_V01;
             break;*/
        }
    }
}

bool AndroidRilAdapter::handleRilSetNetworkPrefResponseV1(
        RilRequestContext* ctx, RIL_Errno e, void* data, size_t data_len) {

    URIL_SVC_TRACE_D(
            "AndroidRilAdapter::handleRilSetNetworkPrefResponseV1()");

    uril_nw_set_pref_resp_msg_v01* iril_rsp =
            (uril_nw_set_pref_resp_msg_v01*) ctx->rsp->payload;

    ctx->rsp->msg_id = URIL_NW_SET_PREF_RESP_V01;
    ctx->rsp->len = sizeof(uril_nw_set_pref_resp_msg_v01);

    rilErrnoToUrilResponse(e, &(iril_rsp->response));
    ctx->isCompleted = true;
    return true;
}

bool AndroidRilAdapter::handleRilGetNetworkPrefResponseV1(
        RilRequestContext* ctx, RIL_Errno e, void* data, size_t data_len) {

    URIL_SVC_TRACE_D(
            "AndroidRilAdapter::handleRilGetNetworkPrefResponseV1()");

    uril_nw_get_pref_resp_msg_v01* iril_rsp =
            (uril_nw_get_pref_resp_msg_v01*) ctx->rsp->payload;

    ctx->rsp->msg_id = URIL_NW_GET_PREF_RESP_V01;
    ctx->rsp->len = sizeof(uril_nw_get_pref_resp_msg_v01);

    rilErrnoToUrilResponse(e, &(iril_rsp->response));
    if (e == RIL_Errno::RIL_E_SUCCESS) {
        iril_rsp->preferred_nw_mode_follows = 1;
        iril_rsp->preferred_nw_mode = RilNwPrefToUrilNwPref(
                (RIL_PreferredNetworkType) *(int*) data);
    }
    ctx->isCompleted = true;
    return true;
}

bool AndroidRilAdapter::handleRilVoiceRegStateResponseV1(RilRequestContext* ctx,
        RIL_Errno e, void* data, size_t data_len) {

    URIL_SVC_TRACE_D(
            "AndroidRilAdapter::handleRilVoiceRegStateResponseV1(): e = %d", e);

    uril_nw_get_registration_status_resp_msg_v01* iril_rsp =
            (uril_nw_get_registration_status_resp_msg_v01*) ctx->rsp->payload;

    ctx->rsp->msg_id = URIL_NW_GET_REGISTRATION_STATUS_RESP_V01;
    ctx->rsp->len = sizeof(uril_nw_get_registration_status_resp_msg_v01);

    rilErrnoToUrilResponse(e, &(iril_rsp->response));

    if (e == RIL_Errno::RIL_E_SUCCESS) {
        URIL_SVC_TRACE_D("VOICE STATUS SUCCESS");
        char** rsp_str = (char**) data;
        int num_entries = data_len / sizeof(char*);
        iril_rsp->voice_registration_follows = rilNwRegToUrilNwReg(
                &(iril_rsp->voice_registration),
                num_entries >= 1 ? rsp_str[0] : nullptr,
                num_entries >= 4 ? rsp_str[3] : nullptr,
                num_entries >= 14 ? rsp_str[13] : nullptr);

        rilCellToUrilCellVoice(iril_rsp,
                num_entries >= 4 ? rsp_str[3] : nullptr,
                num_entries >= 2 ? rsp_str[1] : nullptr,
                num_entries >= 3 ? rsp_str[2] : nullptr,
                num_entries >= 15 ? rsp_str[14] : nullptr);
        ctx->isCompleted = false;
        ctx->rilReqId = RIL_REQUEST_DATA_REGISTRATION_STATE;

        // send RIL request asynchronously to not block the RIL thread that is calling us
        auto asyncFunc = [this](const RilRequestContext* ctx) -> void {
            sendToVendorRil(RIL_REQUEST_DATA_REGISTRATION_STATE, nullptr, 0,
                            ctx);
        };

        std::thread(asyncFunc, ctx).detach();
    } else {
        URIL_SVC_TRACE_D("VOICE STATUS FAILURE");
        ctx->isCompleted = true;
    }
    return true;
}

bool AndroidRilAdapter::handleRilDataRegStateResponseV1(RilRequestContext* ctx,
        RIL_Errno e, void* data, size_t data_len) {

    URIL_SVC_TRACE_D(
            "AndroidRilAdapter::handleRilDataRegStateResponseV1()");

    uril_nw_get_registration_status_resp_msg_v01* iril_rsp =
            (uril_nw_get_registration_status_resp_msg_v01*) ctx->rsp->payload;

    if (iril_rsp == nullptr) {
        return false;
    }

    ctx->rsp->msg_id = URIL_NW_GET_REGISTRATION_STATUS_RESP_V01;
    ctx->rsp->len = sizeof(uril_nw_get_registration_status_resp_msg_v01);

    rilErrnoToUrilResponse(e, &(iril_rsp->response));

    if (e == RIL_Errno::RIL_E_SUCCESS) {
        char** rsp_str = (char**) data;
        int num_entries = data_len / sizeof(char*);
        iril_rsp->data_registration_follows = rilNwRegToUrilNwReg(
                &(iril_rsp->data_registration),
                num_entries >= 1 ? rsp_str[0] : nullptr,
                num_entries >= 4 ? rsp_str[3] : nullptr,
                num_entries >= 14 ? rsp_str[13] : nullptr);

        rilCellToUrilCellData(iril_rsp,
                num_entries >= 4 ? rsp_str[3] : nullptr,
                num_entries >= 2 ? rsp_str[1] : nullptr,
                num_entries >= 3 ? rsp_str[2] : nullptr,
                num_entries >= 15 ? rsp_str[14] : nullptr);

        ctx->isCompleted = false;
        ctx->rilReqId = RIL_REQUEST_OPERATOR;

        // send RIL request asynchronously to not block the RIL thread that is calling us
        auto asyncFunc = [this](const RilRequestContext* ctx) -> void {
            sendToVendorRil(RIL_REQUEST_OPERATOR, nullptr, 0,
                            ctx);
        };

        std::thread(asyncFunc, ctx).detach();
    } else {
        ctx->isCompleted = true;
    }
    return true;
}

bool AndroidRilAdapter::handleRilGetOperatorResponseV1(RilRequestContext* ctx,
        RIL_Errno e, void* data, size_t data_len) {

    URIL_SVC_TRACE_D("AndroidRilAdapter::handleRilGetOperatorResponseV1()");

    uril_nw_get_registration_status_resp_msg_v01* iril_rsp =
            (uril_nw_get_registration_status_resp_msg_v01*) ctx->rsp->payload;

    ctx->rsp->msg_id = URIL_NW_GET_REGISTRATION_STATUS_RESP_V01;
    ctx->rsp->len = sizeof(uril_nw_get_registration_status_resp_msg_v01);

    rilErrnoToUrilResponse(e, &(iril_rsp->response));

    if (e == RIL_Errno::RIL_E_SUCCESS) {
        char** rsp_str = (char**) data;
        int num_entries = data_len / sizeof(char*);

        char* mcc_ptr = nullptr;
        char* mnc_ptr = nullptr;
        if (iril_rsp->nw_cell_identity_gsm_follows) {
            mcc_ptr = iril_rsp->nw_cell_identity_gsm.mcc;
            mnc_ptr = iril_rsp->nw_cell_identity_gsm.mnc;
        } else if (iril_rsp->nw_cell_identity_wcdma_follows) {
            mcc_ptr = iril_rsp->nw_cell_identity_wcdma.mcc;
            mnc_ptr = iril_rsp->nw_cell_identity_wcdma.mnc;
        } else if (iril_rsp->nw_cell_identity_lte_follows) {
            mcc_ptr = iril_rsp->nw_cell_identity_lte.mcc;
            mnc_ptr = iril_rsp->nw_cell_identity_lte.mnc;
        } else if (iril_rsp->nw_cell_identity_tdscdma_follows) {
            mcc_ptr = iril_rsp->nw_cell_identity_tdscdma.mcc;
            mnc_ptr = iril_rsp->nw_cell_identity_tdscdma.mnc;
        }
        if (mcc_ptr != nullptr && num_entries >= 3 && rsp_str[2] != nullptr) {
            strncpy(mcc_ptr, rsp_str[2], 3);
            strncpy(mnc_ptr, rsp_str[2] + 3, 3);
        }
    }
    ctx->isCompleted = true;
    return true;
}

bool AndroidRilAdapter::handleRilAvailableNetworksResponseV1(
        RilRequestContext* ctx, RIL_Errno e, void* data, size_t data_len) {

    URIL_SVC_TRACE_D(
            "AndroidRilAdapter::handleRilAvailableNetworksResponseV1()");

    /** TODO: check pointer alignment / multiple of 4 ? */
    char** rsp_str = (char**) data;
    int num_entries = (data_len / sizeof(char *)) / 4;

    uril_nw_scan_resp_msg_v01* iril_rsp =
            (uril_nw_scan_resp_msg_v01*) ctx->rsp->payload;

    ctx->rsp->msg_id = URIL_NW_SCAN_RESP_V01;
    ctx->rsp->len = sizeof(uril_nw_scan_resp_msg_v01);

    rilErrnoToUrilResponse(e, &(iril_rsp->response));

    if (e == RIL_Errno::RIL_E_SUCCESS) {
        iril_rsp->entries_follows = 1;
        int num_filled = 0;
        for (int i = 0;
                i < num_entries
                        && num_filled < URIL_NW_SCAN_MAX_ENTRY_COUNT_V01;
                i++) {
            if (rilOperatorNameToUrilOperatorName(
                    &iril_rsp->entries[num_filled].operator_name,
                    rsp_str[4 * i + 0], rsp_str[4 * i + 1],
                    rsp_str[4 * i + 2])) {
                if (rsp_str[4 * i + 3] != nullptr) {
                    if (!strcmp(rsp_str[4 * i + 3], "unknown")) {
                        iril_rsp->entries[num_filled].network_status =
                                URIL_NW_NETWORK_STATUS_NONE_V01;
                    } else if (!strcmp(rsp_str[4 * i + 3], "available")) {
                        iril_rsp->entries[num_filled].network_status =
                                URIL_NW_NETWORK_STATUS_AVAILABLE_V01;
                    } else if (!strcmp(rsp_str[4 * i + 3], "current")) {
                        iril_rsp->entries[num_filled].network_status =
                                URIL_NW_NETWORK_STATUS_CURRENT_SERVING_V01;
                    } else if (!strcmp(rsp_str[4 * i + 3], "forbidden")) {
                        iril_rsp->entries[num_filled].network_status =
                                URIL_NW_NETWORK_STATUS_FORBIDDEN_V01;
                    } else
                        continue; /** TODO: error handling ? */
                    num_filled++;
                }
            }
        }
        iril_rsp->entries_len = num_filled;
    }
    ctx->isCompleted = true;
    return true;
}

bool AndroidRilAdapter::handleRilSetScreenStateResponseV1(
        RilRequestContext* ctx, RIL_Errno e, void* data, size_t data_len) {

    URIL_SVC_TRACE_D(
            "AndroidRilAdapter::handleRilSetScreenStateResponseV1()");

    uril_nw_set_screen_state_resp_msg_v01* iril_rsp =
            (uril_nw_set_screen_state_resp_msg_v01*) ctx->rsp->payload;

    ctx->rsp->msg_id = URIL_NW_SET_SCREEN_RESP_V01;
    ctx->rsp->len = sizeof(uril_nw_set_screen_state_resp_msg_v01);

    rilErrnoToUrilResponse(e, &(iril_rsp->response));
    ctx->isCompleted = true;
    return true;
}

bool AndroidRilAdapter::handleRilSetNetworkSelectionResponseV1(
        RilRequestContext* ctx, RIL_Errno e, void* data, size_t data_len) {

    URIL_SVC_TRACE_D(
            "AndroidRilAdapter::handleRilSetNetworkSelectionResponseV1()");

    uril_nw_selection_resp_msg_v01* iril_rsp =
            (uril_nw_selection_resp_msg_v01*) ctx->rsp->payload;

    ctx->rsp->msg_id = URIL_NW_SELECTION_RESP_V01;
    ctx->rsp->len = sizeof(uril_nw_selection_resp_msg_v01);

    rilErrnoToUrilResponse(e, &(iril_rsp->response));
    ctx->isCompleted = true;
    return true;
}

bool AndroidRilAdapter::handleRilGetSignalStrengthResponseV1(
        RilRequestContext* ctx, RIL_Errno e, void* data, size_t data_len) {

    URIL_SVC_TRACE_D(
            "AndroidRilAdapter::handleRilGetSignalStrengthResponseV1()");

    uril_nw_get_signal_strength_resp_msg_v01* iril_rsp =
            (uril_nw_get_signal_strength_resp_msg_v01*) ctx->rsp->payload;

    ctx->rsp->msg_id = URIL_NW_GET_SIGNAL_STRENGTH_RESP_V01;
    ctx->rsp->len = sizeof(uril_nw_get_signal_strength_resp_msg_v01);

    rilErrnoToUrilResponse(e, &(iril_rsp->response));

    if (e == RIL_Errno::RIL_E_SUCCESS) {
        assert(data_len >= sizeof(RIL_SignalStrength_v5));
        RIL_SignalStrength_v10* ril_rsp = (RIL_SignalStrength_v10*) data;

        if (ril_rsp->GW_SignalStrength.signalStrength != 99
                || ril_rsp->GW_SignalStrength.bitErrorRate != 99) {
            /** TODO:
             *    - no way to distinguish between GSM and WCDMA signal strengths
             *    - no indication of format in IRIL interface
             *
             *  Note: maybe use 'RIL_REQUEST_GET_CELL_INFO_LIST' API (but potentially slower and more power consuming).
             */
            iril_rsp->gsm_signal_strength_follows = 1;
            iril_rsp->gsm_signal_strength.rssi =
                    ril_rsp->GW_SignalStrength.signalStrength;
        } else if ((data_len >= sizeof(RIL_SignalStrength_v6))
                && (ril_rsp->LTE_SignalStrength.signalStrength != 99
                        || ril_rsp->LTE_SignalStrength.rsrp != INT_MAX
                        || ril_rsp->LTE_SignalStrength.rsrq != INT_MAX
                        || ril_rsp->LTE_SignalStrength.rssnr != INT_MAX)) {
            iril_rsp->lte_signal_strength_follows = 1;
            iril_rsp->lte_signal_strength.rssi =
                    ril_rsp->LTE_SignalStrength.signalStrength;
            iril_rsp->lte_signal_strength.rsrq =
                    ril_rsp->LTE_SignalStrength.rsrq;
            iril_rsp->lte_signal_strength.rsrp =
                    ril_rsp->LTE_SignalStrength.rsrp;
            iril_rsp->lte_signal_strength.snr =
                    ril_rsp->LTE_SignalStrength.rssnr;
        } else if ((data_len >= sizeof(RIL_SignalStrength_v10))
                && (ril_rsp->TD_SCDMA_SignalStrength.rscp != INT_MAX)) {
            iril_rsp->tdscdma_signal_strength_follows = 1;
            iril_rsp->tdscdma_signal_strength.rscp =
                    ril_rsp->TD_SCDMA_SignalStrength.rscp;
            /** TODO:
             *    - uril_nw_get_signal_strength_resp_msg_v01.uril_nw_tdscdma_signal_strength_t_v01.rssi
             *    - uril_nw_get_signal_strength_resp_msg_v01.uril_nw_tdscdma_signal_strength_t_v01.ecio
             *    - uril_nw_get_signal_strength_resp_msg_v01.uril_nw_tdscdma_signal_strength_t_v01.sinr
             */
        }
    }
    ctx->isCompleted = true;
    return true;
}

bool AndroidRilAdapter::handleRilSetRadioModeV1(RilRequestContext* ctx,
        RIL_Errno e, void* data, size_t data_len) {

    URIL_SVC_TRACE_D("AndroidRilAdapter::handleRilSetRadioModeV1()");

    uril_radio_set_radio_mode_resp_msg_v01* iril_rsp =
            (uril_radio_set_radio_mode_resp_msg_v01*) ctx->rsp->payload;

    ctx->rsp->msg_id = URIL_RADIO_SET_RADIO_MODE_RESP_V01;
    ctx->rsp->len = sizeof(uril_radio_set_radio_mode_resp_msg_v01);

    rilErrnoToUrilResponse(e, &(iril_rsp->response));

    ctx->isCompleted = true;
    return true;
}

bool AndroidRilAdapter::handleRilSetupDataCallV1(RilRequestContext* ctx,
        RIL_Errno e, void* data, size_t data_len) {

    URIL_SVC_TRACE_D("AndroidRilAdapter::handleRilSetupDataCallV1()");

    uril_data_setup_data_call_resp_msg_v01* iril_rsp =
            (uril_data_setup_data_call_resp_msg_v01*) ctx->rsp->payload;

    ctx->rsp->msg_id = URIL_DATA_SETUP_DATA_CALL_RESP_V01;
    ctx->rsp->len = sizeof(uril_data_setup_data_call_resp_msg_v01);

    rilErrnoToUrilResponse(e, &(iril_rsp->response));

    if (e == RIL_Errno::RIL_E_SUCCESS) {
        // Note: need to support RIL_Data_Call_Response_v6 as this is the response used by RPC RIL !!!
        assert(data_len >= sizeof(RIL_Data_Call_Response_v6));
        assert(data_len <= sizeof(RIL_Data_Call_Response_v11));
        RIL_Data_Call_Response_v11 rsp;
        memset(&rsp, 0, sizeof(rsp));
        memcpy(&rsp, data, data_len); // Hack to ease parsing of new fields added by new RIL versions

        rilDataCallResponseToIRilDataCallInfo(&iril_rsp->data_call, &rsp);
    }
    ctx->isCompleted = true;
    return true;
}

bool AndroidRilAdapter::handleRilDeactivateDataCallV1(RilRequestContext* ctx,
        RIL_Errno e, void* data, size_t data_len) {

    URIL_SVC_TRACE_D("AndroidRilAdapter::handleRilDeactivateDataCallV1()");

    uril_data_deactivate_data_call_resp_msg_v01* iril_rsp =
            (uril_data_deactivate_data_call_resp_msg_v01*) ctx->rsp->payload;

    ctx->rsp->msg_id = URIL_DATA_DEACTIVATE_DATA_CALL_RESP_V01;
    ctx->rsp->len = sizeof(uril_data_deactivate_data_call_resp_msg_v01);

    rilErrnoToUrilResponse(e, &(iril_rsp->response));

    ctx->isCompleted = true;
    return true;
}

bool AndroidRilAdapter::handleRilGetDataCallListV1(RilRequestContext* ctx,
        RIL_Errno e, void* data, size_t data_len) {

    URIL_SVC_TRACE_D("AndroidRilAdapter::handleRilGetDataCallListV1()");

    uril_data_get_data_call_list_resp_msg_v01* iril_rsp =
            (uril_data_get_data_call_list_resp_msg_v01*) ctx->rsp->payload;

    ctx->rsp->msg_id = URIL_DATA_GET_DATA_CALL_LIST_RESP_V01;
    ctx->rsp->len = sizeof(uril_data_get_data_call_list_resp_msg_v01);

    rilErrnoToUrilResponse(e, &(iril_rsp->response));
    if (e == RIL_Errno::RIL_E_SUCCESS) {
        int ver = -1;
        int num_calls = -1;
        if (data_len % sizeof(RIL_Data_Call_Response_v6) == 0) {
            ver = 6;
            num_calls = data_len / sizeof(RIL_Data_Call_Response_v6);
        } else if (data_len % sizeof(RIL_Data_Call_Response_v9) == 0) {
            ver = 9;
            num_calls = data_len / sizeof(RIL_Data_Call_Response_v9);
        } else if (data_len % sizeof(RIL_Data_Call_Response_v11) == 0) {
            ver = 11;
            num_calls = data_len / sizeof(RIL_Data_Call_Response_v11);
        }
        assert(ver > 0);

        int i;
        for (i = 0; i < num_calls && i < URIL_DATA_MAX_CALLS_COUNT_V01;
                i++) {
            RIL_Data_Call_Response_v11 rsp;
            memset(&rsp, 0, sizeof(rsp));
            switch (ver) {
            case 6:
                memcpy(&rsp, &((RIL_Data_Call_Response_v6 *) data)[i],
                        sizeof(RIL_Data_Call_Response_v6));
                break;
            case 9:
                memcpy(&rsp, &((RIL_Data_Call_Response_v9 *) data)[i],
                        sizeof(RIL_Data_Call_Response_v9));
                break;
            case 11:
                memcpy(&rsp, &((RIL_Data_Call_Response_v11 *) data)[i],
                        sizeof(RIL_Data_Call_Response_v11));
                break;
            }
            rilDataCallResponseToIRilDataCallInfo(&iril_rsp->data_calls[i],
                    &rsp);
        }
        iril_rsp->data_calls_len = i;
    }
    ctx->isCompleted = true;
    return true;
}

bool AndroidRilAdapter::handleRilSetInitialAttachParamsV1(
        RilRequestContext* ctx, RIL_Errno e, void* data, size_t data_len) {

    URIL_SVC_TRACE_D(
            "AndroidRilAdapter::handleRilSetInitialAttachParamsV1()");

    uril_data_set_initial_params_resp_msg_v01* iril_rsp =
            (uril_data_set_initial_params_resp_msg_v01*) ctx->rsp->payload;

    ctx->rsp->msg_id = URIL_DATA_SET_INITIAL_ATTACH_PARAMS_RESP_V01;
    ctx->rsp->len = sizeof(uril_data_set_initial_params_resp_msg_v01);

    rilErrnoToUrilResponse(e, &(iril_rsp->response));

    ctx->isCompleted = true;
    return true;
}

bool AndroidRilAdapter::handleRilResponse(RIL_Token t, RIL_Errno e, void* data,
        size_t data_len) {

    URIL_SVC_TRACE_D("AndroidRilAdapter::handleRilResponse()");

    bool ret = false;
    RilRequestContext* ctx = (RilRequestContext*) t;

    if (ctx == nullptr) {
        URIL_SVC_TRACE_E("no context");
        return ret;
    }

    if (ctx->rsp == nullptr) {
        ctx->rsp = new uril_protocol_msg_t;
        memset(ctx->rsp, 0, sizeof(uril_protocol_msg_t));
        ctx->rsp->is_indication = 0;
        ctx->rsp->sim_id = ctx->simId;
        ctx->rsp->unique_id = ctx->uniqueId;
    }

    URIL_SVC_TRACE_D("looking for a handler for ril req id: %04x", ctx->rilReqId);

    switch (ctx->rilReqId) {
    case RIL_REQUEST_SET_PREFERRED_NETWORK_TYPE: {
        ret = this->handleRilSetNetworkPrefResponseV1(ctx, e, data, data_len);
    }
        break;
    case RIL_REQUEST_GET_PREFERRED_NETWORK_TYPE: {
        ret = this->handleRilGetNetworkPrefResponseV1(ctx, e, data, data_len);
    }
        break;
    case RIL_REQUEST_VOICE_REGISTRATION_STATE: {
        ret = this->handleRilVoiceRegStateResponseV1(ctx, e, data, data_len);
    }
        break;
    case RIL_REQUEST_DATA_REGISTRATION_STATE: {
        ret = this->handleRilDataRegStateResponseV1(ctx, e, data, data_len);
    }
        break;
    case RIL_REQUEST_OPERATOR: {
        ret = this->handleRilGetOperatorResponseV1(ctx, e, data, data_len);
    }
        break;
    case RIL_REQUEST_QUERY_AVAILABLE_NETWORKS: {
        ret = this->handleRilAvailableNetworksResponseV1(ctx, e, data,
                data_len);
    }
        break;
    case RIL_REQUEST_SCREEN_STATE: {
        ret = this->handleRilSetScreenStateResponseV1(ctx, e, data, data_len);
    }
        break;
    case RIL_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC:
    case RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL: {
        ret = this->handleRilSetNetworkSelectionResponseV1(ctx, e, data,
                data_len);
    }
        break;
    case RIL_REQUEST_SIGNAL_STRENGTH: {
        ret = this->handleRilGetSignalStrengthResponseV1(ctx, e, data,
                data_len);
    }
        break;
    case RIL_REQUEST_RADIO_POWER: {
        ret = this->handleRilSetRadioModeV1(ctx, e, data, data_len);
    }
        break;
    case RIL_REQUEST_SETUP_DATA_CALL: {
        ret = this->handleRilSetupDataCallV1(ctx, e, data, data_len);
    }
        break;
    case RIL_REQUEST_DEACTIVATE_DATA_CALL: {
        ret = this->handleRilDeactivateDataCallV1(ctx, e, data, data_len);
    }
        break;
    case RIL_REQUEST_DATA_CALL_LIST: {
        ret = this->handleRilGetDataCallListV1(ctx, e, data, data_len);
    }
        break;
    case RIL_REQUEST_SET_INITIAL_ATTACH_APN: {
        ret = this->handleRilSetInitialAttachParamsV1(ctx, e, data, data_len);
    }
        break;
    default:
        URIL_SVC_TRACE_E("no handler found for ril req id: %04x", ctx->rilReqId);
    }
    if (ret && ctx->isCompleted) {
        URIL_SVC_TRACE_D("response is complete, sending to upper layers");
        if (ctx->rsp != nullptr) {
            ret = mResponseProcessor->processResponse(ctx->rsp);
            delete ctx->rsp;
        } else {
            URIL_SVC_TRACE_E("unexpected null rsp in context");
        }
        delete ctx;
    }
    return ret;
}

bool AndroidRilAdapter::handleRilUnsolicitedResponse(int unsolResponseId,
        const void* data, size_t data_len) {

    URIL_SVC_TRACE_D("AndroidRilAdapter::handleRilUnsolResponse(%d)", unsolResponseId);

    bool ret = false;
    uril_protocol_msg_t *rsp = new uril_protocol_msg_t;
    memset(rsp, 0, sizeof(uril_protocol_msg_t));
    rsp->is_indication = 1;

    switch (unsolResponseId) {
    case RIL_UNSOL_RESPONSE_RADIO_STATE_CHANGED: {
        rsp->msg_id = URIL_RADIO_MODE_CHANGED_EVENT_IND_V01;
    }
        break;

    case RIL_UNSOL_DATA_CALL_LIST_CHANGED: {
        rsp->msg_id = URIL_DATA_CALL_LIST_CHANGED_EVENT_IND_V01;
    }
        break;

    case RIL_UNSOL_RESPONSE_VOICE_NETWORK_STATE_CHANGED: {
        rsp->msg_id = URIL_NW_REGISTRATION_EVENT_IND_V01;
    }
        break;

    case RIL_UNSOL_SIGNAL_STRENGTH: {
        rsp->msg_id = URIL_NW_SIGNAL_STRENGTH_EVENT_IND_V01;
    }
        break;
    }
    ret = mResponseProcessor->processResponse(rsp);
    delete rsp;

    return ret;
}

const RIL_RadioFunctions* funcs;
AndroidRilAdapter* mInstance;

extern "C" void RIL_onRequestComplete(RIL_Token t, RIL_Errno e, void* response,
        size_t responselen) {
    URIL_SVC_TRACE_D("RIL_onRequestComplete(): e = %d", e);
    mInstance->handleRilResponse(t, e, response, responselen);
}

extern "C" void RIL_onUnsolicitedResponse(int unsolResponse, const void* data,
        size_t datalen) {
    URIL_SVC_TRACE_D("RIL_onUnsolicitedResponse()");
    mInstance->handleRilUnsolicitedResponse(unsolResponse, data, datalen);
}

extern "C" void RIL_requestTimedCallback(RIL_TimedCallback callback,
        void* param, const struct timeval *relativeTime) {
    URIL_SVC_TRACE_D("RIL_requestTimedCallback()");
}

struct RIL_Env rilEnv = { RIL_onRequestComplete, RIL_onUnsolicitedResponse,
        RIL_requestTimedCallback };

AndroidRilAdapter::AndroidRilAdapter(std::string rilLibPath) {
    mRilPath = rilLibPath;
    mInstance = this;
}

void AndroidRilAdapter::setResponseProcessor(
        processor::IResponseProcessor& handler) {
    URIL_SVC_TRACE_D("AndroidRilAdapter::setResponseProcessor()");
    mResponseProcessor = &handler;
}

bool AndroidRilAdapter::init() {

    URIL_SVC_TRACE_D("AndroidRilAdapter::init()");
    mHandle = dlopen(mRilPath.c_str(), RTLD_NOW);

    if (mHandle == nullptr) {
        URIL_SVC_TRACE_D("dlopen failed: %s", dlerror());
        return false;
    }
    const RIL_RadioFunctions *(*rilInit)(const struct RIL_Env *, int, char **);

    rilInit = (const RIL_RadioFunctions *(*)(const struct RIL_Env*, int,
            char**))
            dlsym(mHandle, "RIL_Init");

if(    rilInit == nullptr) {
        URIL_SVC_TRACE_D("RIL_Init not defined or exported in %s", mRilPath.c_str());
        return false;
    }
    funcs = rilInit(&rilEnv, 0, nullptr);
    URIL_SVC_TRACE_D("RIL_Init rilInit completed");

    return true;
}

void* AndroidRilAdapter::makeTokenForMessage(int32_t rilReqId, uint32_t uniqueId, uint8_t simId) {
    URIL_SVC_TRACE_D("AndroidRilAdapter::makeTokenForMessage(%d, %d, %d)",
            rilReqId, uniqueId, simId);
    RilRequestContext* ctx = new RilRequestContext;
    ctx->rilReqId = rilReqId;
    ctx->rsp = nullptr;
    ctx->isCompleted = false;
    ctx->msgId = 0;
    ctx->simId = simId;
    ctx->uniqueId = uniqueId;
    return ctx;
}

bool AndroidRilAdapter::sendToVendorRil(int request, void *data, size_t datalen,
        const void* token) {
    URIL_SVC_TRACE_D("AndroidRilAdapter::sendToVendorRil()");
    funcs->onRequest(request, data, datalen, (void*) token);
    return true;
}

bool AndroidRilAdapter::setNetworkPreferenceV1(
        const uril_nw_set_pref_req_msg_v01* req, uint32_t uniqueId,
        uint8_t simId) {

    URIL_SVC_TRACE_D("AndroidRilAdapter::setNetworkPreferenceV1()");
    int ril_data = urilNwPrefToRilNwPref(req->preferred_nw_mode);
    return sendToVendorRil(RIL_REQUEST_SET_PREFERRED_NETWORK_TYPE, &ril_data,
            sizeof(ril_data), makeTokenForMessage(RIL_REQUEST_SET_PREFERRED_NETWORK_TYPE,
                    uniqueId, simId));
}

bool AndroidRilAdapter::getNetworkPreferenceV1(
        const uril_nw_get_pref_req_msg_v01* req, uint32_t uniqueId,
        uint8_t simId) {

    URIL_SVC_TRACE_D("AndroidRilAdapter::getNetworkPreferenceV1()");
    return sendToVendorRil(RIL_REQUEST_GET_PREFERRED_NETWORK_TYPE, nullptr, 0,
            makeTokenForMessage(RIL_REQUEST_GET_PREFERRED_NETWORK_TYPE, uniqueId, simId));
}

bool AndroidRilAdapter::getNetworkRegistrationStatusV1(
        const uril_nw_get_registration_status_req_msg_v01* req,
        uint32_t uniqueId, uint8_t simId) {
    URIL_SVC_TRACE_D("AndroidRilAdapter::getNetworkRegistrationStatusV1()");
    return sendToVendorRil(RIL_REQUEST_VOICE_REGISTRATION_STATE, nullptr, 0,
            makeTokenForMessage(RIL_REQUEST_VOICE_REGISTRATION_STATE, uniqueId, simId));
}

bool AndroidRilAdapter::scanNetworkV1(const uril_nw_scan_req_msg_v01* req,
        uint32_t uniqueId, uint8_t simId) {
    URIL_SVC_TRACE_D("AndroidRilAdapter::scanNetworkV1()");
    return sendToVendorRil(RIL_REQUEST_QUERY_AVAILABLE_NETWORKS, nullptr, 0,
            makeTokenForMessage(RIL_REQUEST_QUERY_AVAILABLE_NETWORKS, uniqueId, simId));
}

bool AndroidRilAdapter::setScreenStateV1(
        const uril_nw_set_screen_state_req_msg_v01* req, uint32_t uniqueId,
        uint8_t simId) {

    URIL_SVC_TRACE_D("AndroidRilAdapter::setScreenStateV1()");
    int ril_data = req->screen_state != 0;
    return sendToVendorRil(RIL_REQUEST_SCREEN_STATE, &ril_data,
            sizeof(ril_data), makeTokenForMessage(RIL_REQUEST_SCREEN_STATE, uniqueId, simId));
}

bool AndroidRilAdapter::selectNetworkV1(
        const uril_nw_selection_req_msg_v01* req, uint32_t uniqueId,
        uint8_t simId) {

    URIL_SVC_TRACE_D("AndroidRilAdapter::selectNetworkV1()");
    int ril_msg_id;
    char* ril_data_ptr = nullptr;
    char ril_data[3 + 3 + 1];

    switch (req->nw_selection_info.nw_selection_type) {
    case URIL_NW_SELECTION_AUTOMATIC_V01:
        ril_msg_id = RIL_REQUEST_SET_NETWORK_SELECTION_AUTOMATIC;
        break;
    case URIL_NW_SELECTION_MANUAL_V01:
        ril_msg_id = RIL_REQUEST_SET_NETWORK_SELECTION_MANUAL;
        snprintf(ril_data, sizeof(ril_data), "%s%s", req->nw_selection_info.mcc,
                req->nw_selection_info.mnc);
        ril_data_ptr = ril_data;
        break;
    default:
        return false;
    }
    return sendToVendorRil(ril_msg_id, ril_data_ptr,
            ril_data_ptr == nullptr ? 0 : sizeof(ril_data_ptr),
            makeTokenForMessage(ril_msg_id, uniqueId, simId));
}

bool AndroidRilAdapter::getSignalStrengthV1(
        const uril_nw_get_signal_strength_req_msg_v01* req,
        uint32_t uniqueId, uint8_t simId) {

    URIL_SVC_TRACE_D("AndroidRilAdapter::getSignalStrengthV1()");
    return sendToVendorRil(RIL_REQUEST_SIGNAL_STRENGTH, nullptr, 0,
            makeTokenForMessage(RIL_REQUEST_SIGNAL_STRENGTH, uniqueId, simId));
}

bool AndroidRilAdapter::getCellAccessV1(
        const uril_nw_get_cell_access_state_req_msg_v01* req,
        uint32_t uniqueId, uint8_t simId) {
    URIL_SVC_TRACE_D("AndroidRilAdapter::getCellAccessV1()");
    // TODO: need URC here
    return false;
}

bool AndroidRilAdapter::getRadioModeV1(
        const uril_radio_get_radio_mode_req_msg_v01* req,
        uint32_t uniqueId, uint8_t simId) {
    URIL_SVC_TRACE_D("AndroidRilAdapter::getRadioModeV1()");

    // Special handling here as this API is synchronous at RIL side (i.e. it's not message based
    // but function call based).

    auto asyncFunc = [this](const uint32_t& uid, uint8_t& sid) -> void {
        uril_protocol_msg_t* rsp = new uril_protocol_msg_t;
            memset(rsp, 0, sizeof(uril_protocol_msg_t));

        uril_radio_get_radio_mode_resp_msg_v01 *iril_rsp =
                (uril_radio_get_radio_mode_resp_msg_v01 *) rsp->payload;

        iril_rsp->response.result = URIL_RESULT_SUCCESS_V01;
        iril_rsp->response.error = URIL_ERROR_SUCCESS_V01;

        rsp->msg_id = URIL_RADIO_GET_RADIO_MODE_RESP_V01;
        rsp->len = sizeof(uril_radio_get_radio_mode_resp_msg_v01);
        rsp->unique_id = uid;
        rsp->sim_id = sid;

        RIL_RadioState radio_state = funcs->onStateRequest();

        iril_rsp->radio_mode_follows = 1;
        switch (radio_state) {
        case RIL_RadioState::RADIO_STATE_OFF:
            iril_rsp->radio_mode = URIL_RADIO_MODE_OFFLINE_V01;
            break;
        case RIL_RadioState::RADIO_STATE_UNAVAILABLE:
            iril_rsp->radio_mode = URIL_RADIO_MODE_UNAVAILABLE_V01;
            break;
        case RIL_RadioState::RADIO_STATE_ON:
            iril_rsp->radio_mode = URIL_RADIO_MODE_ONLINE_V01;
            break;
        default:
            assert(0);
            break;
        }
        mResponseProcessor->processResponse(rsp);
        delete rsp;
    };

    std::thread(asyncFunc, std::ref(uniqueId), std::ref(simId)).detach();

    return true;
}

bool AndroidRilAdapter::setRadioModeV1(
        const uril_radio_set_radio_mode_req_msg_v01* req,
        uint32_t uniqueId, uint8_t simId) {
    URIL_SVC_TRACE_D("AndroidRilAdapter::setRadioModeV1()");

    int ril_data;
    if (req->radio_mode
            == uril_radio_mode_t_v01::URIL_RADIO_MODE_OFFLINE_V01)
        ril_data = 0;
    else if (req->radio_mode
            == uril_radio_mode_t_v01::URIL_RADIO_MODE_ONLINE_V01)
        ril_data = 1;
    else
        assert(0);

    return sendToVendorRil(RIL_REQUEST_RADIO_POWER, &ril_data,
            sizeof(&ril_data), makeTokenForMessage(RIL_REQUEST_RADIO_POWER, uniqueId, simId));
}

bool AndroidRilAdapter::setDataProfilesV1(
        const uril_data_set_data_profiles_req_msg_v01 *req,
        uint32_t uniqueId, uint8_t simId) {
    // TODO
    return false;
}

bool AndroidRilAdapter::setupDataCallV1(
        const uril_data_setup_data_call_req_msg_v01 *req,
        uint32_t uniqueId, uint8_t simId) {
    URIL_SVC_TRACE_D("AndroidRilAdapter::setupDataCallV1()");

    const char *ril_data[8];
    char ril_radio_tech[16];
    char ril_data_profile[16];
    char ril_auth_type[16];
    snprintf(ril_radio_tech, sizeof(ril_radio_tech), "%d",
            urilDataBearerTechTypeToRilRadioTech(req->rat));
    // TODO check that uril_data_setup_data_call_req_msg_v01.profile_id matches Android's definition
    snprintf(ril_data_profile, sizeof(ril_data_profile), "%d", req->profile_id);
    snprintf(ril_auth_type, sizeof(ril_auth_type), "%d",
            urilAuthTypeToRilAuthType(req->auth_type));
    ril_data[0] = ril_radio_tech;
    ril_data[1] = ril_data_profile;
    ril_data[2] = req->apn;
    // TODO check if we need to put nullptr or if empty string is OK
    ril_data[3] = req->user;
    ril_data[4] = req->password;
    ril_data[5] = ril_auth_type;
    ril_data[6] = urilPdpTypeToRilPdpType(req->pdp_type);
    ril_data[7] = nullptr;

    return sendToVendorRil(RIL_REQUEST_SETUP_DATA_CALL, ril_data,
            sizeof(ril_data), makeTokenForMessage(RIL_REQUEST_SETUP_DATA_CALL, uniqueId, simId));
}

bool AndroidRilAdapter::deactivateDataCallV1(
        const uril_data_deactivate_data_call_req_msg_v01 *req,
        uint32_t uniqueId, uint8_t simId) {
    URIL_SVC_TRACE_D("AndroidRilAdapter::deactivateDataCallV1()");
    const char *ril_data[2];

    char ril_cid[8];
    snprintf(ril_cid, sizeof(ril_cid), "%d", req->cid);
    ril_data[0] = ril_cid;

    if (req->reason == URIL_DATA_END_REASON_RADIO_OFF)
        ril_data[1] = "1";
    else
        ril_data[1] = "0";

    return sendToVendorRil(RIL_REQUEST_DEACTIVATE_DATA_CALL, ril_data,
            sizeof(ril_data), makeTokenForMessage(RIL_REQUEST_DEACTIVATE_DATA_CALL,
                    uniqueId, simId));
}

bool AndroidRilAdapter::getDataCallListV1(
        const uril_data_get_data_call_list_req_msg_v01 *req,
        uint32_t uniqueId, uint8_t simId) {
    URIL_SVC_TRACE_D("AndroidRilAdapter::deactivateDataCallV1()");

    return sendToVendorRil(RIL_REQUEST_DATA_CALL_LIST, nullptr, 0,
            makeTokenForMessage(RIL_REQUEST_DATA_CALL_LIST, uniqueId, simId));
}

bool AndroidRilAdapter::setInitialAttachParamsV1(
        const uril_data_set_initial_params_req_msg_v01 *req,
        uint32_t uniqueId, uint8_t simId) {
    URIL_SVC_TRACE_D("AndroidRilAdapter::setInitialAttachParamsV1()");
    RIL_InitialAttachApn ril_data;

    // Note: copy of string is needed as the pointers are not constant in RIL interface ...
    ril_data.apn = strdup(req->apn);
    ril_data.protocol = strdup(urilPdpTypeToRilPdpType(req->pdp_type));
    ril_data.authtype = urilAuthTypeToRilAuthType(req->auth_type);
    ril_data.username = strdup(req->user);
    ril_data.password = strdup(req->password);

    bool ret = sendToVendorRil(RIL_REQUEST_SET_INITIAL_ATTACH_APN, &ril_data,
            sizeof(ril_data), makeTokenForMessage(RIL_REQUEST_SET_INITIAL_ATTACH_APN,
                    uniqueId, simId));

    delete ril_data.apn;
    delete ril_data.protocol;
    delete ril_data.username;
    delete ril_data.password;
    return ret;
}

}
}
}
