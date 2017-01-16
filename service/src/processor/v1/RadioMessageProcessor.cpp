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

#include <assert.h>
#include "../../uril_svc_log.h"
#include "RadioMessageProcessor.h"
#include "uril_protocol.h"
#include "uril_common_v01.h"
#include "uril_radio_v01.h"
#include "IModemAdapter.h"

namespace uril {
namespace processor {
namespace v1 {

RadioMessageProcessor::RadioMessageProcessor(adapters::IModemAdapter* adapter,
        IResponseProcessor* responseProcessor) {
    assert(adapter != nullptr);
    assert(responseProcessor != nullptr);
    mModemAdapter = adapter;
    mModemAdapter->setResponseProcessor(*responseProcessor);
}

RadioMessageProcessor::~RadioMessageProcessor() {

}

bool RadioMessageProcessor::processRequest(const uril_protocol_msg_t* msg) {
    assert(msg != nullptr);

    URIL_SVC_TRACE_D("RadioMessageProcessor::processRequest()");

    if (msg == nullptr) {
        URIL_SVC_TRACE_E("unexpected null message");
        return false;
    }

    URIL_SVC_TRACE_E("handling message id: %04x", msg->msg_id);

    switch (msg->msg_id) {
    case URIL_RADIO_SET_RADIO_MODE_REQ_V01: {
        uril_radio_set_radio_mode_req_msg_v01* iril_msg =
                (uril_radio_set_radio_mode_req_msg_v01*) msg->payload;
        return this->mModemAdapter->setRadioModeV1(iril_msg, msg->unique_id, msg->sim_id);
    }
    case URIL_RADIO_GET_RADIO_MODE_REQ_V01: {
        uril_radio_get_radio_mode_req_msg_v01* iril_msg =
                (uril_radio_get_radio_mode_req_msg_v01*) msg->payload;
        return this->mModemAdapter->getRadioModeV1(iril_msg, msg->unique_id, msg->sim_id);
    }
    default:
        break;
    }
    return false;
}

}
}
}
