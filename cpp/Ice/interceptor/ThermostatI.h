//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#ifndef THERMOSTAT_I_H
#define THERMOSTAT_I_H

#include <mutex>
#include <Interceptor.h>

class ThermostatI : public Demo::Thermostat
{
public:

    virtual float getTemp(const Ice::Current&) override;
    virtual void setTemp(float, const Ice::Current&) override;
    virtual void shutdown(const Ice::Current&) override;

private:

    // Temperature in Celsius.
    float _temperature = 23.5f;
    std::mutex _thermostatLock;
};

#endif
