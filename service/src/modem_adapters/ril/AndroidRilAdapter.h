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

#ifndef ANDROIDRILADAPTER_H_
#define ANDROIDRILADAPTER_H_

#include "IModemAdapter.h"
#include "IResponseProcessor.h"

#ifndef RIL_SHLIB
#define RIL_SHLIB   1
#endif
#include <telephony/ril.h>

namespace uril {
namespace adapters {
namespace ril {

class AndroidRilAdapter: public IModemAdapter {

protected:
    typedef struct {
        int32_t rilReqId;
        uint32_t uniqueId;
        uint8_t simId;
        uint32_t msgId;
        uril_protocol_msg_t* rsp;
        bool isCompleted;
    } RilRequestContext;

    processor::IResponseProcessor* mResponseProcessor = nullptr;
    std::string mRilPath;
    void* mHandle = nullptr;

protected:
    virtual void* makeTokenForMessage(int32_t rilReqId, uint32_t uniqueId, uint8_t simId);
    virtual bool sendToVendorRil(int request, void *data, size_t datalen,
            const void* token);
    virtual bool handleRilGetSignalStrengthResponseV1(RilRequestContext* ctx,
            RIL_Errno e, void* data, size_t data_len);
    virtual bool handleRilSetNetworkSelectionResponseV1(RilRequestContext* ctx,
            RIL_Errno e, void* data, size_t data_len);
    virtual bool handleRilSetScreenStateResponseV1(RilRequestContext* ctx,
            RIL_Errno e, void* data, size_t data_len);
    virtual bool handleRilAvailableNetworksResponseV1(RilRequestContext* ctx,
            RIL_Errno e, void* data, size_t data_len);
    virtual bool handleRilGetOperatorResponseV1(RilRequestContext* ctx,
            RIL_Errno e, void* data, size_t data_len);
    virtual bool handleRilDataRegStateResponseV1(RilRequestContext* ctx,
            RIL_Errno e, void* data, size_t data_len);
    virtual bool handleRilVoiceRegStateResponseV1(RilRequestContext* ctx,
            RIL_Errno e, void* data, size_t data_len);
    virtual bool handleRilGetNetworkPrefResponseV1(RilRequestContext* ctx,
            RIL_Errno e, void* data, size_t data_len);
    virtual bool handleRilSetNetworkPrefResponseV1(RilRequestContext* ctx,
            RIL_Errno e, void* data, size_t data_len);
    virtual bool handleRilSetRadioModeV1(RilRequestContext* ctx,
            RIL_Errno e, void* data, size_t data_len);
    virtual bool handleRilSetupDataCallV1(RilRequestContext* ctx,
            RIL_Errno e, void* data, size_t data_len);
    virtual bool handleRilDeactivateDataCallV1(RilRequestContext* ctx,
            RIL_Errno e, void* data, size_t data_len);
    virtual bool handleRilGetDataCallListV1(RilRequestContext* ctx,
            RIL_Errno e, void* data, size_t data_len);
    virtual bool handleRilSetInitialAttachParamsV1(RilRequestContext* ctx,
            RIL_Errno e, void* data, size_t data_len);

public:
    AndroidRilAdapter(std::string rilLibPath);
    virtual ~AndroidRilAdapter() {
    }
    virtual bool handleRilResponse(RIL_Token t, RIL_Errno e, void* data,
            size_t data_len);
    virtual bool handleRilUnsolicitedResponse(int unsolResponseId, const void* data,
            size_t data_len);
    virtual void setResponseProcessor(processor::IResponseProcessor& handler);
    virtual bool init();
    virtual bool setNetworkPreferenceV1(const uril_nw_set_pref_req_msg_v01* req, uint32_t uniqueId, uint8_t simId);
    virtual bool getNetworkPreferenceV1(const uril_nw_get_pref_req_msg_v01* req, uint32_t uniqueId, uint8_t simId);
    virtual bool getNetworkRegistrationStatusV1(const uril_nw_get_registration_status_req_msg_v01* req, uint32_t uniqueId, uint8_t simId);
    virtual bool scanNetworkV1(const uril_nw_scan_req_msg_v01* req, uint32_t uniqueId, uint8_t simId);
    virtual bool setScreenStateV1(const uril_nw_set_screen_state_req_msg_v01* req, uint32_t uniqueId, uint8_t simId);
    virtual bool selectNetworkV1(const uril_nw_selection_req_msg_v01* req, uint32_t uniqueId, uint8_t simId);
    virtual bool getSignalStrengthV1(const uril_nw_get_signal_strength_req_msg_v01* req, uint32_t uniqueId, uint8_t simId);
    virtual bool getCellAccessV1(const uril_nw_get_cell_access_state_req_msg_v01* req, uint32_t uniqueId, uint8_t simId);

    virtual bool getRadioModeV1(const uril_radio_get_radio_mode_req_msg_v01* req, uint32_t uniqueId, uint8_t simId);
    virtual bool setRadioModeV1(const uril_radio_set_radio_mode_req_msg_v01* req, uint32_t uniqueId, uint8_t simId);

    virtual bool setDataProfilesV1(const uril_data_set_data_profiles_req_msg_v01 *req, uint32_t uniqueId, uint8_t simId);
    virtual bool setupDataCallV1(const uril_data_setup_data_call_req_msg_v01 *req, uint32_t uniqueId, uint8_t simId);
    virtual bool deactivateDataCallV1(const uril_data_deactivate_data_call_req_msg_v01 *req, uint32_t uniqueId, uint8_t simId);
    virtual bool getDataCallListV1(const uril_data_get_data_call_list_req_msg_v01 *req, uint32_t uniqueId, uint8_t simId);
    virtual bool setInitialAttachParamsV1(const uril_data_set_initial_params_req_msg_v01 *req, uint32_t uniqueId, uint8_t simId);
};


}
}
}

#endif /* ANDROIDRILADAPTER_H_ */
