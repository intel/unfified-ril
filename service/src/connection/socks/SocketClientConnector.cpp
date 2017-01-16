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

#include <thread>
#include <mutex>
#include <vector>
#include <condition_variable>
#include <algorithm>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "SocketClientConnector.h"
#include "IClientConnector.h"

#include "../../uril_svc_log.h"
#include "uril_protocol.h"

namespace uril {
namespace connection {
namespace socks {

void SocketClientConnector::acceptLoop() {
    if (listen(mListeningSocket, mMaxClientsCount) < 0) {
        URIL_SVC_TRACE_E("listen failed: %s", strerror(errno));
        return;
    }
    int accepted_socket = -1;

    do {
        URIL_SVC_TRACE_D("waiting for client connections...");
        accepted_socket = accept(mListeningSocket, NULL, NULL);
        if (accepted_socket >= 0) {
            URIL_SVC_TRACE_D("accepting new client");
            size_t protocol_len = sizeof(uril_protocol_msg_t);
            if (setsockopt(accepted_socket, SOL_SOCKET, SO_RCVLOWAT,
                    &protocol_len, protocol_len) < 0) {
                URIL_SVC_TRACE_E("setsockopt failed: %s", strerror(errno));
            }
            SocketClientConnection* connection =
                                new SocketClientConnection(accepted_socket);
            {
                std::lock_guard<std::mutex> lock(mConnectionsLock);
                mConnections.push_back(connection);
            }
            connection->registerObserver(*this); // subscribe to know when disconnected
            {
                for (IClientConnectorObserver* obs : mObservers) {
                    obs->onNewConnection(*this, *connection);
                }
            }
            URIL_SVC_TRACE_D("new connection notified to all observers");
            // launch the thread ultimately to ensure observers wont miss first messages
            connection->ignitPump();
        }
    } while (accepted_socket >= 0);
    URIL_SVC_TRACE_D("exiting loop because accept failed");
}

SocketClientConnector::SocketClientConnector(int maxClientsCount)
    : mMaxClientsCount(maxClientsCount) {
}

SocketClientConnector::~SocketClientConnector() {
    this->stop();
}

std::vector<IClientConnector::IClientConnection*> SocketClientConnector::getConnections() const {
    return mConnections;
}

void SocketClientConnector::registerObserver(IClientConnectorObserver& observer) {
    URIL_SVC_TRACE_D("SocketClientConnector::registerObserver()");
    std::lock_guard<std::mutex> lock(mObserversLock);
    mObservers.push_back(&observer);
}

void SocketClientConnector::unregisterObserver(IClientConnectorObserver& observer) {
    URIL_SVC_TRACE_D("SocketClientConnector::unregisterObserver()");
    std::lock_guard<std::mutex> lock(mObserversLock);
    mObservers.erase(std::remove(mObservers.begin(), mObservers.end(), &observer), mObservers.end());
}

void SocketClientConnector::onClientDisconnected(IClientConnector::IClientConnection& sender) {
    URIL_SVC_TRACE_D("SocketClientConnector::onClientDisconnected()");
    sender.unregisterObserver(*this);
    std::lock_guard<std::mutex> lock(mConnectionsLock);
    IClientConnector::IClientConnection* ptr = &sender;
    mConnections.erase(std::remove(mConnections.begin(), mConnections.end(), ptr), mConnections.end());
}

void SocketClientConnector::onMessageReceived(IClientConnector::IClientConnection& sender,
            const uril_protocol_msg_t* msg) {
    // nothing to do here because new messages are handled at handlers layer
}

bool SocketClientConnector::stop() {
    URIL_SVC_TRACE_D("SocketClientConnector::stop()");
    if (mIsRunning) {
        if (close(mListeningSocket) == 0) {
            mThread->join();
            delete mThread;
            mThread = nullptr;
            return true;
        } else {
            URIL_SVC_TRACE_E("unable to close socket: %s", strerror(errno));
        }
    }
    return false;
}

SocketClientConnector::SocketClientConnection::
    SocketClientConnection(int commSocket)
    : mCommSocket(commSocket) {
}

SocketClientConnector::SocketClientConnection::
    SocketClientConnection(const SocketClientConnection& that)
    : mCommSocket(that.mCommSocket), mObservers(that.mObservers) {
}

SocketClientConnector::SocketClientConnection::~SocketClientConnection() {
    if (mCommSocket != -1) {
        close(mCommSocket);
    }
    if (mThread != nullptr) {
        mThread->join();
        delete mThread;
    }
}

void SocketClientConnector::SocketClientConnection::pumpLoop() {
    URIL_SVC_TRACE_D("SocketClientConnection::pumpLoop()");
    while (true) {
        uril_protocol_msg_t msg;
        memset(&msg, 0, sizeof(msg));
        ssize_t received = recv(mCommSocket, &msg, sizeof(msg), MSG_WAITALL);
        if (received < 0) {
            URIL_SVC_TRACE_E("recv failed: %s", strerror(errno));
            break;
        }
        if (received == 0) {
            URIL_SVC_TRACE_D("client disconnected");
            for (IClientConnectionObserver* obs : mObservers) {
                obs->onClientDisconnected(*this);
            }
            delete this; // delete after all observer notified
            break;
        } else {
            URIL_SVC_TRACE_D("received something from client");
            for (IClientConnectionObserver* obs : mObservers) {
                obs->onMessageReceived(*this, &msg);
            }
        }
        usleep(1000); // prevent from CPU overload in case of abnormal incoming data
    }
    URIL_SVC_TRACE_D("exiting pumpLoop()");
}

bool SocketClientConnector::SocketClientConnection::ignitPump() {
    URIL_SVC_TRACE_D("SocketClientConnection::ignitPump()");
    mThread = new std::thread(
            &SocketClientConnector::SocketClientConnection::pumpLoop, this);
    return true;
}

bool SocketClientConnector::SocketClientConnection::
    sendMessageToClient(const uril_protocol_msg_t* msg) {
    URIL_SVC_TRACE_D("SocketClientConnection::sendMessageToClient()");
    std::lock_guard<std::mutex> lock(mSocketLock);
#if 0
    URIL_SVC_TRACE_D("DUMP SERVICE");
    char tempHex[sizeof(uril_protocol_msg_t) * 2 + 1];
    char* buf_ptr = tempHex;

    memset(tempHex, 0, sizeof(tempHex));

    for (size_t i = 0; i < sizeof(uril_protocol_msg_t); i++) {
        uint8_t* val = ((uint8_t*) msg) + i;
        buf_ptr += sprintf(buf_ptr, "%02X", *val);
    }
    *(buf_ptr) = '\0';

    URIL_SVC_TRACE_D("\r%s\r", tempHex);
#endif
    URIL_SVC_TRACE_D("msg_id %d :: unique_id %d :: len %d", msg->msg_id,msg->unique_id,msg->len);
    URIL_SVC_TRACE_D("sending %zd bytes", sizeof(*msg));
    if (send(mCommSocket, msg, sizeof(*msg), MSG_DONTWAIT) < 0) {
        return false;
    }
    return true;
}

void SocketClientConnector::SocketClientConnection::
    registerObserver(IClientConnectionObserver& observer) {
    URIL_SVC_TRACE_D("SocketClientConnection::registerObserver()");
    std::lock_guard<std::mutex> lock(mObserversLock);
    mObservers.push_back(&observer);
}

void SocketClientConnector::SocketClientConnection::
    unregisterObserver(IClientConnectionObserver& observer) {
    URIL_SVC_TRACE_D("SocketClientConnection::unregisterObserver()");
    std::lock_guard<std::mutex> lock(mObserversLock);
    mObservers.erase(std::remove(mObservers.begin(), mObservers.end(), &observer), mObservers.end());
}

}
}
}
