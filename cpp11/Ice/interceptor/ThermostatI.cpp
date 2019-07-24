//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include <Ice/Ice.h>
#include <ThermostatI.h>

float
ThermostatI::getTemp(const Ice::Current& current) const
{
    return _temperature;
}

void
ThermostatI::setTemp(float temp, const Ice::Current& current)
{
    _temperature = temp;
}
