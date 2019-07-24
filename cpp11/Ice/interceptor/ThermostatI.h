//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#ifndef THERMOSTAT_I_H
#define THERMOSTAT_I_H

#include <Interceptor.h>

class ThermostatI : public Demo::Thermostat
{
public:

    virtual float getTemp(const Ice::Current&) const override;
    virtual void setTemp(float, const Ice::Current&) override;

private:

    // Temperature in Celsius.
    float _temperature;
};

#endif
