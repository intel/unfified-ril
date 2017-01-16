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

#include <stdio.h>

#include "gtest/gtest.h"
#include "uril_client.h"
#include "uril_client_internal.h"
#include "service_connector.h"
#include "mock_service_connector.h"
#include "uril_nw_v01.h"
#include "uril_data_v01.h"
#include "uril_protocol.h"

uril_client_handle_t test_execute_async_dummy_request_hndl = 0;
uint32_t test_execute_async_dummy_request_msg_id = 0;
uint8_t test_execute_async_dummy_request_sim_id = 0;
uril_nw_get_cell_access_state_resp_msg_v01 test_execute_async_dummy_request_resp;

uint32_t test_ind_msg_id = 0;

service_connector_t* service_connector = NULL;

class UrilClientTestSuite: public testing::Test {

public:
    void SetUp() {
        service_connector = mock_service_connector_create();
        uril_client_set_service_connector(service_connector);
    }

    void TearDown() {
        uril_client_set_service_connector(nullptr);
        delete service_connector;
        memset(&test_execute_async_dummy_request_resp, 0,
                sizeof(test_execute_async_dummy_request_resp));
    }
};

TEST_F(UrilClientTestSuite, test_init_with_null_args) {

    int ret = uril_client_init(nullptr, nullptr, nullptr);

    EXPECT_LT(ret, 0);
}

TEST_F(UrilClientTestSuite, test_init_with_null_callbacks) {

    uril_client_handle_t handle = 0;

    int ret = uril_client_init(&handle, nullptr, nullptr);

    EXPECT_EQ(ret, 0);
}

TEST_F(UrilClientTestSuite, test_execute_async_without_init) {

    uril_client_handle_t handle = 0;

    int ret = uril_client_execute_command_async(handle, 0, 0,nullptr, 0, nullptr, nullptr);

    EXPECT_LT(ret, 0);
}

static void test_execute_async_null_request_cb(uril_client_handle_t hndl,
        uint8_t sim_id, uint32_t msg_id, const void* resp, size_t resp_len,
        const void* client_data) {
    printf("test_execute_async_null_request_cb(%d, %d, %d, ...)", hndl, sim_id,
            msg_id);
}

TEST_F(UrilClientTestSuite, test_execute_async_null_request) {

    uril_client_handle_t handle = 0;

    int ret = uril_client_init(&handle, nullptr, nullptr);

    EXPECT_EQ(ret, 0);

    ret = uril_client_execute_command_async(handle, 0, 32, nullptr, 0,
            test_execute_async_null_request_cb, nullptr);

    EXPECT_LT(ret, 0);
}

static void test_execute_async_dummy_request_cb(uril_client_handle_t hndl,
        uint8_t sim_id, uint32_t msg_id, const void* resp, size_t resp_len,
        const void* client_data) {
    printf("test_execute_async_dummy_request_cb(%d, %d, %d, ...)", hndl, sim_id,
            msg_id);

    test_execute_async_dummy_request_hndl = hndl;
    test_execute_async_dummy_request_msg_id = msg_id;
    test_execute_async_dummy_request_sim_id = sim_id;

    if (msg_id == URIL_NW_GET_CELL_ACCESS_STATE_REQ_V01) {
        memcpy(&test_execute_async_dummy_request_resp, resp,
                sizeof(test_execute_async_dummy_request_resp));
    } else {
        memset(&test_execute_async_dummy_request_resp, 0,
                sizeof(test_execute_async_dummy_request_resp));
    }
}

TEST_F(UrilClientTestSuite, test_execute_async_dummy_request) {

    uril_client_handle_t handle = 0;

    int ret = uril_client_init(&handle, nullptr, nullptr);

    EXPECT_EQ(ret, 0);

    uril_nw_get_cell_access_state_req_msg_v01 req;
    uint32_t msg_id = URIL_NW_GET_CELL_ACCESS_STATE_REQ_V01;

    ret = URIL_CLIENT_EXECUTE_COMMAND_ASYNC(handle, 0, msg_id, &req,
        test_execute_async_dummy_request_cb, nullptr);

EXPECT_EQ(ret, 0);
    EXPECT_EQ(test_execute_async_dummy_request_hndl, handle);
    EXPECT_EQ(test_execute_async_dummy_request_msg_id,
            URIL_NW_GET_CELL_ACCESS_STATE_RESP_V01);
    EXPECT_EQ(test_execute_async_dummy_request_sim_id, 0);

    EXPECT_EQ(test_execute_async_dummy_request_resp.response.result, URIL_RESULT_SUCCESS_V01);
    EXPECT_EQ(test_execute_async_dummy_request_resp.nw_cell_access_state_follows, 1);
    EXPECT_EQ(test_execute_async_dummy_request_resp.nw_cell_access_state, URIL_NW_CELL_ACCESS_ALL_CALLS_V01);
}

TEST_F(UrilClientTestSuite, test_execute_async_dummy_request_with_default_cb) {

    uril_client_handle_t handle = 0;

    int ret = uril_client_init(&handle, nullptr, test_execute_async_dummy_request_cb);

    EXPECT_EQ(ret, 0);

    uril_nw_get_cell_access_state_req_msg_v01 req;
    uint32_t msg_id = URIL_NW_GET_CELL_ACCESS_STATE_REQ_V01;

    ret = URIL_CLIENT_EXECUTE_COMMAND_ASYNC(handle, 0, msg_id, &req, nullptr,
        nullptr);

EXPECT_EQ(ret, 0);
    EXPECT_EQ(test_execute_async_dummy_request_hndl, handle);
    EXPECT_EQ(test_execute_async_dummy_request_msg_id,
            URIL_NW_GET_CELL_ACCESS_STATE_RESP_V01);
    EXPECT_EQ(test_execute_async_dummy_request_sim_id, 0);

    EXPECT_EQ(test_execute_async_dummy_request_resp.response.result, URIL_RESULT_SUCCESS_V01);
    EXPECT_EQ(test_execute_async_dummy_request_resp.nw_cell_access_state_follows, 1);
    EXPECT_EQ(test_execute_async_dummy_request_resp.nw_cell_access_state, URIL_NW_CELL_ACCESS_ALL_CALLS_V01);
}

TEST_F(UrilClientTestSuite, test_execute_sync_dummy_request) {

    uril_client_handle_t handle = 0;

    int ret = uril_client_init(&handle, nullptr, test_execute_async_dummy_request_cb);

    EXPECT_EQ(ret, 0);

    uril_nw_get_cell_access_state_req_msg_v01 req;
    uril_nw_get_cell_access_state_resp_msg_v01 resp;
    uint32_t msg_id = URIL_NW_GET_CELL_ACCESS_STATE_REQ_V01;

    ret = URIL_CLIENT_EXECUTE_COMMAND_SYNC(handle, 0, msg_id, &req, &resp, 5000);

EXPECT_EQ(ret, 0);

EXPECT_EQ(resp.response.result, URIL_RESULT_SUCCESS_V01);
EXPECT_EQ(resp.nw_cell_access_state_follows, 1);
EXPECT_EQ(resp.nw_cell_access_state, URIL_NW_CELL_ACCESS_ALL_CALLS_V01);
}

void test_ind_cb(uril_client_handle_t hndl, uint8_t sim_id,
    uint32_t msg_id) {
printf("test_ind_cb(%d, %d, %d)", hndl, sim_id, msg_id);

test_ind_msg_id = msg_id;
}

TEST_F(UrilClientTestSuite, test_ind) {

uril_client_handle_t handle = 0;

int ret = uril_client_init(&handle, test_ind_cb, nullptr);

EXPECT_EQ(ret, 0);

uint32_t msg_id = URIL_NW_SIGNAL_STRENGTH_EVENT_IND_V01;
uril_protocol_msg_t ind;
ind.sim_id = 0;
ind.is_indication = 1;
ind.msg_id = msg_id;

service_connector->on_receive(service_connector, sizeof(ind), &ind);

EXPECT_EQ(test_ind_msg_id, msg_id);
}

int main(int argc, char** argv) {
testing::InitGoogleTest(&argc, argv);

return RUN_ALL_TESTS();
}
