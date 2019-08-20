//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include <Ice/Ice.h>
#include <ThermostatI.h>

using namespace std;

float
ThermostatI::getTemp(const Ice::Current&)
{
    lock_guard<mutex> lock(_thermostatLock);
    return _temperature;
}

void
ThermostatI::setTemp(float temp, const Ice::Current&)
{
    lock_guard<mutex> lock(_thermostatLock);
    _temperature = temp;
}

void
ThermostatI::shutdown(const Ice::Current& current)
{
    cout << "Shutting down..." << endl;
    current.adapter->getCommunicator()->shutdown();
}
