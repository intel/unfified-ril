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
#include "MessageTracker.h"
#include "IMessageProcessor.h"
#include "uril_protocol.h"
#include "IClientConnector.h"
#include "uril_svc_log.h"

namespace uril {

MessageTracker::MessageTracker(IClientConnector* connector) {
    assert(connector != nullptr);
    mConnector = connector;
    connector->registerObserver(*this);
}

MessageTracker::~MessageTracker() {
}

void MessageTracker::setNextProcessor(processor::IRequestProcessor* nextProcessor) {
    mNextProcessor = nextProcessor;
}

void MessageTracker::onNewConnection(IClientConnector& sender,
            IClientConnector::IClientConnection& newConnection) {
    URIL_SVC_TRACE_D("onNewConnection()");
    newConnection.registerObserver(*this);
}

void MessageTracker::onClientDisconnected(IClientConnector::IClientConnection& sender) {
    URIL_SVC_TRACE_D("onClientDisconnected()");
    sender.unregisterObserver(*this);
}

void MessageTracker::onMessageReceived(IClientConnector::IClientConnection& sender,
        const uril_protocol_msg_t* msg) {
    URIL_SVC_TRACE_D("onMessageReceived()");
    this->trackAndDispatchRequest(sender, msg);
}

bool MessageTracker::trackAndDispatchRequest(IClientConnection& connection,
        const uril_protocol_msg_t* msg) {
    assert(msg != nullptr);
    {
        std::lock_guard<std::mutex> lock(mRequestsLock);
        mPendingRequests[msg->unique_id] = &connection;
    }
    if (mNextProcessor != nullptr) {
        return mNextProcessor->processRequest(msg);
    }
    return false;
}

bool MessageTracker::processResponse(const uril_protocol_msg_t* msg) {
    URIL_SVC_TRACE_D("MessageTracker::processResponse()");
    bool ret = true;
    if (msg != nullptr) {
        if (msg->is_indication) {
            URIL_SVC_TRACE_D("message is an indication");
            for (IClientConnection* connection : mConnector->getConnections()) {
                ret &= connection->sendMessageToClient(msg);
            }
        } else {
            URIL_SVC_TRACE_D("message is a response");
            uint32_t unique_id = msg->unique_id;
            URIL_SVC_TRACE_D("received a response for unique id = %d", unique_id);
            IClientConnection* connection = nullptr;
            {
                std::lock_guard<std::mutex> lock(mRequestsLock);
                connection = mPendingRequests[unique_id];
                mPendingRequests.erase(unique_id);
            }
            if (connection != nullptr) {
                URIL_SVC_TRACE_D("found a connection for %d", unique_id);
                ret = connection->sendMessageToClient(msg);
            } else {
                URIL_SVC_TRACE_E("could not find a connection for %d", unique_id);
            }
        }
    } else {
        ret = false;
        URIL_SVC_TRACE_E("unexpected null message");
    }
    return ret;
}

}
