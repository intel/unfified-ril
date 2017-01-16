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

#ifndef SOCKETCLIENTCONNECTOR_H_
#define SOCKETCLIENTCONNECTOR_H_

#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "IClientConnector.h"

namespace uril {
namespace connection {
namespace socks {

class SocketClientConnector : public IClientConnector, private IClientConnectionObserver {

public:

    class SocketClientConnection : public IClientConnector::IClientConnection {
    private:
        int mCommSocket = -1;
        std::vector<IClientConnectionObserver*> mObservers;
        std::mutex mObserversLock;
        std::mutex mSocketLock;
        std::thread* mThread = nullptr;

        void pumpLoop();

    public:
        SocketClientConnection(int commSocket);
        SocketClientConnection(const SocketClientConnection& that);
        SocketClientConnection& operator=(const SocketClientConnection& that) {
            mCommSocket = that.mCommSocket;
            mObservers = that.mObservers;
            mThread = that.mThread;
            return *this;
        }
        virtual ~SocketClientConnection();

        bool sendMessageToClient(const uril_protocol_msg_t* msg);
        void registerObserver(IClientConnectionObserver& observer);
        void unregisterObserver(IClientConnectionObserver& observer);
        bool ignitPump();
    };

private:
    std::vector<IClientConnection*> mConnections;
    std::mutex mConnectionsLock;
    std::vector<IClientConnectorObserver*> mObservers;
    std::mutex mObserversLock;

protected:
    int mListeningSocket = -1;
    bool mIsRunning = 0;
    std::thread* mThread = nullptr;
    int mMaxClientsCount = 0;

    virtual void acceptLoop();

public:
    SocketClientConnector(int maxClientsCount);
    virtual ~SocketClientConnector();
    std::vector<IClientConnection*> getConnections() const;
    void registerObserver(IClientConnectorObserver& observer);
    void unregisterObserver(IClientConnectorObserver& observer);
    virtual bool stop();

protected:
    virtual void onClientDisconnected(IClientConnector::IClientConnection& sender);
    virtual void onMessageReceived(IClientConnector::IClientConnection& sender,
                const uril_protocol_msg_t* msg);
};

}
}
}

#endif /* SOCKETCLIENTCONNECTOR_H_ */
