//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Ice.interceptor.Demo.*;

class ThermostatI implements Thermostat
{
    @Override
    public float getTemp(com.zeroc.Ice.Current current)
    {
        return _temperature;
    }

    @Override
    public void setTemp(float temp, com.zeroc.Ice.Current current)
    {
        _temperature = temp;
    }

    @Override
    public void shutdown(com.zeroc.Ice.Current current)
    {
        System.out.println("Shutting down...");
        current.adapter.getCommunicator().shutdown();
    }

    // Temperature in Celsius.
    private float _temperature = 23.5f;
}
