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

#ifndef IMODEMADAPTER_H_
#define IMODEMADAPTER_H_

#include "uril_nw_v01.h"
#include "uril_radio_v01.h"
#include "uril_data_v01.h"
#include "IResponseProcessor.h"
#include "uril_protocol.h"

namespace uril {
namespace adapters {

    class IModemAdapter {

    public:
        IModemAdapter() {}
        virtual ~IModemAdapter() {}
        virtual void setResponseProcessor(processor::IResponseProcessor& handler) = 0;
        virtual bool init() = 0;

        virtual bool setNetworkPreferenceV1(const uril_nw_set_pref_req_msg_v01* req, uint32_t uniqueId, uint8_t simId) = 0;
        virtual bool getNetworkPreferenceV1(const uril_nw_get_pref_req_msg_v01* req, uint32_t uniqueId, uint8_t simId) = 0;
        virtual bool getNetworkRegistrationStatusV1(const uril_nw_get_registration_status_req_msg_v01* req, uint32_t uniqueId, uint8_t simId) = 0;
        virtual bool scanNetworkV1(const uril_nw_scan_req_msg_v01* req, uint32_t uniqueId, uint8_t simId) = 0;
        virtual bool setScreenStateV1(const uril_nw_set_screen_state_req_msg_v01* req, uint32_t uniqueId, uint8_t simId) = 0;
        virtual bool selectNetworkV1(const uril_nw_selection_req_msg_v01* req, uint32_t uniqueId, uint8_t simId) = 0;
        virtual bool getSignalStrengthV1(const uril_nw_get_signal_strength_req_msg_v01* req, uint32_t uniqueId, uint8_t simId) = 0;
        virtual bool getCellAccessV1(const uril_nw_get_cell_access_state_req_msg_v01* req, uint32_t uniqueId, uint8_t simId) = 0;

        virtual bool getRadioModeV1(const uril_radio_get_radio_mode_req_msg_v01* req, uint32_t uniqueId, uint8_t simId) = 0;
        virtual bool setRadioModeV1(const uril_radio_set_radio_mode_req_msg_v01* req, uint32_t uniqueId, uint8_t simId) = 0;

        virtual bool setDataProfilesV1(const uril_data_set_data_profiles_req_msg_v01 *req, uint32_t uniqueId, uint8_t simId) = 0;
        virtual bool setupDataCallV1(const uril_data_setup_data_call_req_msg_v01 *req, uint32_t uniqueId, uint8_t simId) = 0;
        virtual bool deactivateDataCallV1(const uril_data_deactivate_data_call_req_msg_v01 *req, uint32_t uniqueId, uint8_t simId) = 0;
        virtual bool getDataCallListV1(const uril_data_get_data_call_list_req_msg_v01 *req, uint32_t uniqueId, uint8_t simId) = 0;
        virtual bool setInitialAttachParamsV1(const uril_data_set_initial_params_req_msg_v01 *req, uint32_t uniqueId, uint8_t simId) = 0;
    };
}
}

#endif /* IMODEMADAPTER_H_ */
