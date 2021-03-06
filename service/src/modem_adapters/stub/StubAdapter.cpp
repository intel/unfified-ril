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

#include "../../uril_svc_log.h"
#include "StubAdapter.h"
#include "IResponseProcessor.h"
#include "uril_common_v01.h"

namespace uril {
namespace adapters {
namespace stub {

StubAdapter::StubAdapter() {
}

void StubAdapter::setResponseProcessor(processor::IResponseProcessor& handler) {
    mResponseProcessor = &handler;
}

bool StubAdapter::init() {
    return true;
}

bool StubAdapter::handleRequest(const uril_protocol_msg_t* req) {
    uril_protocol_msg_t rsp;
    memset(&rsp, 0, sizeof(rsp));

    uril_response_t_v01* rspHeader = (uril_response_t_v01*) rsp.payload;
    rspHeader->result = uril_result_t_v01::URIL_RESULT_SUCCESS_V01;

    rsp.msg_id = req->msg_id;
    rsp.unique_id = req->unique_id;

    if (mResponseProcessor != nullptr) {
        mResponseProcessor->processResponse(&rsp);
    } else {
        return false;
    }
    return true;
}

}
}
}
