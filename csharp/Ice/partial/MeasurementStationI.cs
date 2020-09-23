// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using Ice;
using System;
using System.Linq;
using System.Collections.Generic;

public class MeasurementStationI : MeasurementStationDisp_
{
    public override Distance getDistance(Current current = null)
    {
        var distance = new Distance();
        distance.Yards = random.NextDouble() * 1200;
        return distance;
    }

    public override Speed getSpeed(Current current = null)
    {
        var speed = new Speed();
        speed.MilesPerHour = random.NextDouble() * 120;
        return speed;
    }

    public override void shutdown(Current current)
    {
        current.adapter.getCommunicator().shutdown();
    }

    private Random random = new Random();
}
