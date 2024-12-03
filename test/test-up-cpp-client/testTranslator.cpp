/*********************************************************************
* Copyright (c) 9-7-2024 Cummins Inc
*
* This program and the accompanying materials are made
* available under the terms of the Eclipse Public License 2.0
* which is available at https://www.eclipse.org/legal/epl-2.0/
*
* SPDX-License-Identifier: EPL-2.0
**********************************************************************/
#include <gmock/gmock.h>
#include <nlohmann/json.hpp>
#include "testIMqttWrapper.h"
#include "apiconstants.h"
#include "Application.h"
#include "udsTestPayloads.h"


class ApplicationDerived : public Application
{
public:
    ApplicationDerived(std::shared_ptr<InterContainerMessenger> protocolClient) : Application(protocolClient)
    {
        
    }
    virtual ~ApplicationDerived(){};
  

};
