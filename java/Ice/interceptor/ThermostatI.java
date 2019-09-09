//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Ice.interceptor.Demo.*;
import com.zeroc.Ice.Current;

class ThermostatI implements Thermostat
{
    @Override
    public synchronized float getTemp(Current current)
    {
        return _temperature;
    }

    @Override
    public synchronized void setTemp(float temp, Current current)
    {
        _temperature = temp;
    }

    @Override
    public void shutdown(Current current)
    {
        System.out.println("Shutting down...");
        current.adapter.getCommunicator().shutdown();
    }

    // Temperature in Celsius.
    private float _temperature = 23.5f;
}
