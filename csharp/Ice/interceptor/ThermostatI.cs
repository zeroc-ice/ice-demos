//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

using Demo;

public class ThermostatI : ThermostatDisp_
{
    public override float getTemp(Ice.Current current)
    {
        return temperature;
    }

    public override void setTemp(float temp, Ice.Current current)
    {
        temperature = temp;
    }

    public override void shutdown(Ice.Current current)
    {
        System.out.println("Shutting down...");
        current.adapter.getCommunicator().shutdown();
    }

    private float temperature = 23.5f;
}
