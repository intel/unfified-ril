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

#include <memory>
#include <vector>
#include "CompositeRequestProcessor.h"
#include "IRequestProcessor.h"
#include "../uril_svc_log.h"
#include "uril_protocol.h"

namespace uril {
namespace processor {

CompositeRequestProcessor::CompositeRequestProcessor() {
}

CompositeRequestProcessor::~CompositeRequestProcessor() {
    for (IRequestProcessor* child : mChildren) {
        delete child;
    }
}

bool CompositeRequestProcessor::processRequest(const uril_protocol_msg_t* msg) {
    bool ret = false;

    for (IRequestProcessor* child : mChildren) {
        ret = child->processRequest(msg);
        if (ret) {
            URIL_SVC_TRACE_D("found a processor: %s, ignoring others", child->getName().c_str());
            break;
        }
    }
    if (!ret) {
        URIL_SVC_TRACE_E("no processor found");
    }
    return ret;
}

CompositeRequestProcessor* CompositeRequestProcessor::addChildRequestProcessor(IRequestProcessor* processor) {
    mChildren.push_back(processor);
    return this;
}

}
}
