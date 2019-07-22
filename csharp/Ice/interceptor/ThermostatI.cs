//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

using Demo;

class ThermostatI : ThermostatDisp_
{
    public override float getTemp(Ice.Current current)
    {
        return temperature;
    }

    public override void setTemp(float temp, Ice.Current current)
    {
        temperature = temp;
    }

    // Temperature in Celsius.
    private float temperature = 23.5f;
}
