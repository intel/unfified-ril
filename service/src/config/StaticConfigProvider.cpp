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

#include <string.h>

#include "StaticConfigProvider.h"

namespace uril {
namespace config {

StaticConfigProvider::StaticConfigProvider() {
}

StaticConfigProvider::~StaticConfigProvider() {
}

bool StaticConfigProvider::getConfig(uril_config_t* config) {

    if (config != nullptr) {
        memset(config, 0, sizeof(*config));
        config->connector.local.maxClientsCount = 5;
        config->connector.tcp.binding_ip = 0; // 0.0.0.0 = ANY
        config->connector.tcp.maxClientsCount = 5;
        config->connector.tcp.binding_port = 5060;

        config->modem_adapter.modem_adapter_type = modem_adapter_t::RIL_MODEM_ADAPTER;
        config->modem_adapter.ril_modem.vendor_ril_path = "librpc-ril.so";

#if CONNECTOR_TCP
        config->connector.connector_type = connector_type_t::TCP;
#else
        config->connector.connector_type = connector_type_t::LOCAL_UNIX;
#endif

    }
    return true;
}

}
}
