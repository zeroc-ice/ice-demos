//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include <Ice/Ice.h>
#include <ThermostatI.h>

float
ThermostatI::getTemp(const Ice::Current&)
{
    std::lock_guard<std::mutex> lock(_thermostatLock);
    return _temperature;
}

void
ThermostatI::setTemp(float temp, const Ice::Current&)
{
    std::lock_guard<std::mutex> lock(_thermostatLock);
    _temperature = temp;
}

void
ThermostatI::shutdown(const Ice::Current& current)
{
    std::cout << "Shutting down..." << std::endl;
    current.adapter->getCommunicator()->shutdown();
}
