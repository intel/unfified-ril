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

#ifndef TCPSOCKETCLIENTCONNECTOR_H_
#define TCPSOCKETCLIENTCONNECTOR_H_

#include <string>
#include <stdint.h>

#include "SocketClientConnector.h"

namespace uril {
namespace connection {
namespace socks {

class TcpSocketClientConnector : public SocketClientConnector {

public:

    class TcpSocketClientConnection : public SocketClientConnector::SocketClientConnection {

    public:
        TcpSocketClientConnection(int commSocket);
        TcpSocketClientConnection(const TcpSocketClientConnection& that);
        TcpSocketClientConnection& operator=(const TcpSocketClientConnection& that);
        virtual ~TcpSocketClientConnection();
    };

private:
    uint32_t mPort = -1;
    uint32_t mIpBinding = -1;

public:
    TcpSocketClientConnector(int maxClientsCount, uint32_t ipBinding, uint32_t port);
    virtual ~TcpSocketClientConnector();

    virtual std::string getName() const;
    virtual bool start();
};

}
}
}

#endif /* TCPSOCKETCLIENTCONNECTOR_H_ */
