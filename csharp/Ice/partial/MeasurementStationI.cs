// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using Ice;
using System;
using System.Threading.Tasks;
using System.Linq;
using System.Collections.Generic;

public class MeasurementStationI : MeasurementStationDisp_
{
    public override Distance getDistance(Current current = null, CancellationToken cancel)
    {
        var distance = new Distance();
        distance.Yards = random.NextDouble() * 1200;
        return distance;
    }

    public override Speed getSpeed(Current current = null, CancellationToken cancel)
    {
        var speed = new Speed();
        speed.MilesPerHour = random.NextDouble() * 120;
        return speed;
    }

    public override void shutdown(Current current, CancellationToken cancel)
    {
        current.adapter.getCommunicator().shutdown();
    }

    private Random random = new Random();
}
