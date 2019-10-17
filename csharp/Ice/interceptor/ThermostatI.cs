//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

using Demo;
using System;

class ThermostatI : ThermostatDisp_
{
    public override float getTemp(Ice.Current current)
    {
        lock(this)
        {
            return _temperature;
        }
    }

    public override void setTemp(float temp, Ice.Current current)
    {
        lock(this)
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
}
