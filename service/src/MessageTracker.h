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

#ifndef MESSAGETRACKER_H_
#define MESSAGETRACKER_H_

#include <mutex>
#include <unordered_map>
#include "IResponseProcessor.h"
#include "IRequestProcessor.h"
#include "uril_protocol.h"
#include "IClientConnector.h"

namespace uril {

using IClientConnector = connection::IClientConnector;
using IClientConnection = IClientConnector::IClientConnection;

class MessageTracker : public processor::IResponseProcessor,
        public connection::IClientConnectorObserver,
        public connection::IClientConnectionObserver {
private:
    std::unordered_map<uint32_t, IClientConnection*> mPendingRequests;
    processor::IRequestProcessor* mNextProcessor = nullptr;
    std::mutex mRequestsLock;
    IClientConnector* mConnector = nullptr;
    bool trackAndDispatchRequest(IClientConnection& connection,
            const uril_protocol_msg_t* msg);

public:
    MessageTracker(IClientConnector* connector);
    virtual ~MessageTracker();
    std::string getName() { return std::string("MessageTracker"); }
    void setNextProcessor(processor::IRequestProcessor* nextProcessor);
    bool processResponse(const uril_protocol_msg_t* msg);
    void onNewConnection(IClientConnector& sender,
            IClientConnector::IClientConnection& newConnection);
    void onClientDisconnected(IClientConnector::IClientConnection& sender);
    void onMessageReceived(IClientConnector::IClientConnection& sender,
            const uril_protocol_msg_t* msg);
};

}
#endif /* MESSAGETRACKER_H_ */
