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
#include <algorithm>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "UnixSocketClientConnector.h"
#include "IClientConnector.h"

#include "../../uril_svc_log.h"
#include "uril_protocol.h"

#define UNIX_SOCKET_NAME                "/tmp/uril_sock"

namespace uril {
namespace connection {
namespace socks {

UnixSocketClientConnector::UnixSocketClientConnector(int maxClientsCount)
    : SocketClientConnector(maxClientsCount) {
    URIL_SVC_TRACE_D("UnixSocketClientConnector::UnixSocketClientConnector()");
}

UnixSocketClientConnector::~UnixSocketClientConnector() {
}

bool UnixSocketClientConnector::start() {
    URIL_SVC_TRACE_D("UnixSocketClientConnector::start()");
    struct sockaddr_un serveraddr;

    if (!mIsRunning) {
        mListeningSocket = socket(AF_UNIX, SOCK_STREAM, 0);
        if (mListeningSocket < 0) {
            URIL_SVC_TRACE_E("socket failed: %s", strerror(errno));
            return false;
        }

        memset(&serveraddr, 0, sizeof(serveraddr));
        serveraddr.sun_family = AF_UNIX;
        strcpy(serveraddr.sun_path, UNIX_SOCKET_NAME);

        unlink(UNIX_SOCKET_NAME);
        if(bind(mListeningSocket, (struct sockaddr *)&serveraddr, SUN_LEN(&serveraddr)) < 0) {
            URIL_SVC_TRACE_E("bind failed: %s", strerror(errno));
            return false;
        }

        mThread = new std::thread(&UnixSocketClientConnector::acceptLoop, this);
        mIsRunning = true;
    } else {
        URIL_SVC_TRACE_E("connector is already running");
        return false;
    }

    return true;
}

std::string UnixSocketClientConnector::getName() const {
    return std::string("UnixSocketClientConnector");
}

UnixSocketClientConnector::UnixSocketClientConnection::
    UnixSocketClientConnection(int commSocket)
    : SocketClientConnection(commSocket) {
}

UnixSocketClientConnector::UnixSocketClientConnection::
    UnixSocketClientConnection(const UnixSocketClientConnection& that)
    : SocketClientConnection(that) {
}

UnixSocketClientConnector::UnixSocketClientConnection::~UnixSocketClientConnection() {
}

}
}
}
