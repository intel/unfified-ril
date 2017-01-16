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

#include <sys/prctl.h>
#include <unistd.h>

#include "IClientConnector.h"
#include "IMessageProcessor.h"
#include "IModemAdapter.h"
#include "IRequestProcessor.h"
#include "IResponseProcessor.h"

#include "uril_svc_log.h"

#include "config/StaticConfigProvider.h"
#include "UrilConfigBasedFactory.h"

#include "MessageTracker.h"

int main(int argc, char** argv) {

    //enable stack dump
    prctl(PR_SET_DUMPABLE, 1);

    URIL_SVC_TRACE_D("URIL Service starting");

    uril::connection::IClientConnector* connector = nullptr;
    uril::adapters::IModemAdapter* adapter = nullptr;
    uril::processor::IRequestProcessor* requestProcessor = nullptr;
    uril::MessageTracker* msgTracker = nullptr;

    {
        uril::config::uril_config_t config;

        URIL_SVC_TRACE_D("Loading config...");
        if (!uril::config::StaticConfigProvider().getConfig(&config)) {
            URIL_SVC_TRACE_E("Config load failed, exiting.");
            return -1;
        }
        uril::UrilConfigBasedFactory factory(config);

        URIL_SVC_TRACE_D("Creating connector...");
        connector = factory.createClientConnector();
        URIL_SVC_TRACE_D("Done");
        URIL_SVC_TRACE_D("Creating adapter...");
        adapter = factory.createModemAdapter();
        URIL_SVC_TRACE_D("Done");
        URIL_SVC_TRACE_D("Creating messages tracker...");
        msgTracker = new uril::MessageTracker(connector);
        URIL_SVC_TRACE_D("Done");
        URIL_SVC_TRACE_D("Creating request processor...");
        requestProcessor = factory.createRequestProcessor(adapter, msgTracker);
        URIL_SVC_TRACE_D("Done");
    }
    msgTracker->setNextProcessor(requestProcessor);

    if (adapter != nullptr) {
        URIL_SVC_TRACE_D("Initializing adapter...");
        if (adapter->init()) {
            URIL_SVC_TRACE_D("Done");
        } else {
            URIL_SVC_TRACE_E("Failed");
            return -1;
        }
    }

    URIL_SVC_TRACE_D("Starting connector...");
    if (!connector->start()) {
        URIL_SVC_TRACE_E("Failed");
        return -1;
    } else {
        URIL_SVC_TRACE_D("Done");
    }
    URIL_SVC_TRACE_D("URIL service started");
    while (true) {
        sleep(UINT32_MAX);
    }
    // should never come here but let's free resources...
    connector->stop();
    delete requestProcessor;
    delete msgTracker;
    delete adapter;
    delete connector;
    return -1;
}
