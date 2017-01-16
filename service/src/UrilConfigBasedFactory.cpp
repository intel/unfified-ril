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

#include <string>

#include "UrilConfigBasedFactory.h"
#include "IModemAdapter.h"
#include "IClientConnector.h"
#include "connection/socks/UnixSocketClientConnector.h"
#include "connection/socks/TcpSocketClientConnector.h"
#include "modem_adapters/ril/AndroidRilAdapter.h"
#include "modem_adapters/stub/StubAdapter.h"
#include "processor/CompositeRequestProcessor.h"
#include "processor/v1/NetworkMessageProcessor.h"
#include "processor/v1/RadioMessageProcessor.h"
#include "processor/v1/DataMessageProcessor.h"

namespace uril {

UrilConfigBasedFactory::UrilConfigBasedFactory(
        uril::config::uril_config_t config) {
    mConfig = config;
}

UrilConfigBasedFactory::~UrilConfigBasedFactory() {
}

IClientConnector* UrilConfigBasedFactory::createClientConnector() {
    switch (mConfig.connector.connector_type) {
    case uril::config::connector_type_t::TCP:
        return new connection::socks::TcpSocketClientConnector(
                mConfig.connector.tcp.maxClientsCount,
                mConfig.connector.tcp.binding_ip,
                mConfig.connector.tcp.binding_port);
    case uril::config::connector_type_t::LOCAL_UNIX:
    default:
        return new connection::socks::UnixSocketClientConnector(
                mConfig.connector.local.maxClientsCount);
    }
    return nullptr;
}

adapters::IModemAdapter* UrilConfigBasedFactory::createModemAdapter() {
    switch (mConfig.modem_adapter.modem_adapter_type) {
    case uril::config::modem_adapter_t::RIL_MODEM_ADAPTER:
        return new adapters::ril::AndroidRilAdapter(
                std::string(mConfig.modem_adapter.ril_modem.vendor_ril_path));
    default:
        break;
    /*case uril::config::modem_adapter_t::STUB:
        return new adapters::stub::StubAdapter();*/
    }
    return nullptr;
}

processor::IRequestProcessor* UrilConfigBasedFactory::createRequestProcessor(
                adapters::IModemAdapter* modemAdapter,
                processor::IResponseProcessor* responseProcessor) {

    uril::processor::CompositeRequestProcessor* ret =
            new uril::processor::CompositeRequestProcessor();
    ret->addChildRequestProcessor(
            new uril::processor::v1::NetworkMessageProcessor(modemAdapter, responseProcessor));
    ret->addChildRequestProcessor(
            new uril::processor::v1::RadioMessageProcessor(modemAdapter, responseProcessor));
    ret->addChildRequestProcessor(
            new uril::processor::v1::DataMessageProcessor(modemAdapter, responseProcessor));
    return ret;
}

} /* namespace uril */
