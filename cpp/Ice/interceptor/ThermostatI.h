// Copyright (c) ZeroC, Inc.

#ifndef THERMOSTAT_I_H
#define THERMOSTAT_I_H

#include <Interceptor.h>
#include <mutex>

class ThermostatI : public Demo::Thermostat
{
public:
    float getTemp(const Ice::Current&) override;
    void setTemp(float, const Ice::Current&) override;
    void shutdown(const Ice::Current&) override;

private:
    // Temperature in Celsius.
    float _temperature = 23.5f;
    std::mutex _thermostatLock;
};

#endif
