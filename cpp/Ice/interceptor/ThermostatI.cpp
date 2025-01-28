// Copyright (c) ZeroC, Inc.

#include "ThermostatI.h"
#include <Ice/Ice.h>
#include <iostream>

using namespace std;

float
ThermostatI::getTemp(const Ice::Current&)
{
    const lock_guard<mutex> lock(_thermostatLock);
    return _temperature;
}

void
ThermostatI::setTemp(float temp, const Ice::Current&)
{
    const lock_guard<mutex> lock(_thermostatLock);
    _temperature = temp;
}

void
ThermostatI::shutdown(const Ice::Current& current)
{
    cout << "Shutting down..." << endl;
    current.adapter->getCommunicator()->shutdown();
}
