// Copyright (c) ZeroC, Inc.

using Demo;

internal class MeasurementStationI : MeasurementStationDisp_
{
    public override Distance getDistance(Ice.Current current = null)
    {
        var distance = new Distance
        {
            Yards = random.NextDouble() * 1200
        };
        return distance;
    }

    public override Speed getSpeed(Ice.Current current = null)
    {
        var speed = new Speed
        {
            MilesPerHour = random.NextDouble() * 120
        };
        return speed;
    }

    public override void shutdown(Ice.Current current)
    {
        current.adapter.getCommunicator().shutdown();
    }

    private Random random = new Random();
}
