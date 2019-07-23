//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

using Demo;

class ThermostatI : ThermostatDisp_
{
    public override float getTemp(Ice.Current current)
    {
        return _temperature;
    }

    public override void setTemp(float temp, Ice.Current current)
    {
        _temperature = temp;
    }

    // Temperature in Celsius.
    private float _temperature = 23.5f;
}
