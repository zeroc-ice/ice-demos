//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

using Demo;
using System;

class ThermostatI : ThermostatDisp_
{
    public override float getTemp(Ice.Current current)
    {
        lock(_thermostatLock)
        {
            return _temperature;
        }
    }

    public override void setTemp(float temp, Ice.Current current)
    {
        lock(_thermostatLock)
        {
            _temperature = temp;
        }
    }

    public override void shutdown(Ice.Current current)
    {
        Console.Out.WriteLine("Shutting down...");
        current.adapter.getCommunicator().shutdown();
    }

    // Temperature in Celsius.
    private float _temperature = 23.5f;
    private readonly object _thermostatLock = new object();
}
