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

#ifndef ICLIENTCONNECTOR_H_
#define ICLIENTCONNECTOR_H_

#include <vector>
#include <string>

#include "uril_protocol.h"

namespace uril {
namespace connection {

struct IClientConnectorObserver;
struct IClientConnectionObserver;

class IClientConnector {
public:

    class IClientConnection {

    public:
        IClientConnection() {}
        virtual ~IClientConnection() {}
        virtual bool ignitPump() = 0;
        virtual bool sendMessageToClient(const uril_protocol_msg_t* msg) = 0;
        virtual void registerObserver(IClientConnectionObserver& observer) = 0;
        virtual void unregisterObserver(IClientConnectionObserver& observer) = 0;
    };

    IClientConnector() {}
    virtual ~IClientConnector() {}

    virtual bool start() = 0;
    virtual bool stop() = 0;
    virtual std::string getName() const = 0;
    virtual std::vector<IClientConnection*> getConnections() const = 0;
    virtual void registerObserver(IClientConnectorObserver& observer) = 0;
    virtual void unregisterObserver(IClientConnectorObserver& observer) = 0;
};

struct IClientConnectorObserver {
    virtual void onNewConnection(IClientConnector& sender,
            IClientConnector::IClientConnection& newConnection) = 0;

protected:
    ~IClientConnectorObserver() {}
};

struct IClientConnectionObserver {
    virtual void onClientDisconnected(IClientConnector::IClientConnection& sender) = 0;
    virtual void onMessageReceived(IClientConnector::IClientConnection& sender,
            const uril_protocol_msg_t* msg) = 0;
protected:
    ~IClientConnectionObserver() {}
};

}
}

#endif /* ICLIENTCONNECTOR_H_ */
