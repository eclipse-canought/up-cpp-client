/*********************************************************************
* Copyright (c) 9-7-2024 Cummins Inc
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "udsTestPayloads.h"
#include "observer.h"
#include "Application.h"
#include "iintercontainer-messenger.h"
#include "testIMqttWrapper.h"
#include "nlohmann/json.hpp"



#ifndef DISABLE_OBSERVER_TEST

class MockApplication : public Application{
public:
    MockApplication(std::shared_ptr<InterContainerMessenger> Client) : Application(Client){ }
    MOCK_METHOD(std::string, OnIncomingMessage, (const std::string &,
                                  const jsonType &));
   
};

class TestObserver : public ::testing::Test
{
public:
    void SetUp() override
    {
        Client = std::make_shared<MockIMqttWrapper>(global::SERVER_ADDRESS, global::CLIENT_ID);
        EXPECT_CALL(*Client, Connect(::testing::_)).WillRepeatedly(::testing::Return());

        EXPECT_CALL(*Client, ListenTo(::testing::_))
            .Times(::testing::AnyNumber());

        observer = std::make_unique<MockApplication>(Client);
    }

    void TearDown() override
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); /*500ms sleep to avoid dead-lock*/
    }

protected:
    std::shared_ptr<MockIMqttWrapper> Client = nullptr;
    std::unique_ptr<Observer> observer = nullptr;   
};


TEST_F(TestObserver, Forwardlaltest)
{
    google::protobuf::Struct responsepayload;
    auto &fields = *responsepayload.mutable_fields();
    fields["appid"].set_string_value("global::CLIENT_ID");
    fields["connectionid"].set_string_value("0x12345678");
    fields["sequenceno"].set_string_value("1");
    fields["responsecode"].set_string_value("0");
    std::string finalResponse;
    responsepayload.SerializeToString(&finalResponse);
    const std::string TOPIC = global::publish::OPEN_COMM_CHANNEL;
    const IprotocolMessage message(TOPIC, finalResponse);    
    
    std::thread initalizethread([this](){ observer->Initialize();});
   
    std::this_thread::sleep_for(std::chrono::milliseconds(10)); 
    std::thread threadobserver([this,message](){observer->OnMessage(message);}); 
   
    threadobserver.join();
    initalizethread.join();
   
}
#endif
