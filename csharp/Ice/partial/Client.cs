// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using System;

public class Client
{
    public static int Main(string[] args)
    {
        int status = 0;

        try
        {
            //
            // using statement - communicator is automatically destroyed
            // at the end of this statement
            //
            using(var communicator = Ice.Util.initialize(ref args, "config.client"))
            {
                //
                // Destroy the communicator on Ctrl+C or Ctrl+Break
                //
                Console.CancelKeyPress += (sender, eventArgs) => communicator.destroy();

                if(args.Length > 0)
                {
                    Console.Error.WriteLine("too many arguments");
                    status = 1;
                }
                else
                {
                    status = run(communicator);
                }
            }
        }
        catch(Exception ex)
        {
            Console.Error.WriteLine(ex);
            status = 1;
        }

        return status;
    }

    private static int run(Ice.Communicator communicator)
    {
        var station = MeasurementStationPrxHelper.checkedCast(communicator.propertyToProxy("MeasurementStation.Proxy"));
        if(station == null)
        {
            Console.Error.WriteLine("invalid proxy");
            return 1;
        }

        //
        // Get a random measurement.
        //
        var distance = station.getDistance();

        //
        // Using the properties defined in the partial struct Distance we can access
        // the value represented by this object in a more convenient way.
        //
        Console.WriteLine("distance: {0} meters equal {1} yards", Math.Round(distance.Meters, 4),
                                                                  Math.Round(distance.Yards, 4));

        //
        // This works equally well for types created locally.
        //
        distance = new Distance(1);
        Console.WriteLine("distance: {0} meters equal {1} yards", Math.Round(distance.Meters, 4),
                                                                  Math.Round(distance.Yards, 4));

        //
        // The same apply for generated class types like Speed.
        //
        var speed = station.getSpeed();

        Console.WriteLine("speed: {0} m/s is equal {1} km/h {2} mph",
            Math.Round(speed.MetersPerSecond, 4),
            Math.Round(speed.KilometersPerHour, 4),
            Math.Round(speed.MilesPerHour, 4));

        //
        // Again this works equally well for objects created locally.
        //
        speed = new Speed(1);

        Console.WriteLine("speed: {0} m/s is equal {1} km/h {2} mph",
            Math.Round(speed.MetersPerSecond, 4),
            Math.Round(speed.KilometersPerHour, 4),
            Math.Round(speed.MilesPerHour, 4));

        speed.KilometersPerHour = 1;

        Console.WriteLine("speed: {0} m/s is equal {1} km/h {2} mph",
            Math.Round(speed.MetersPerSecond, 4),
            Math.Round(speed.KilometersPerHour, 4),
            Math.Round(speed.MilesPerHour, 4));

        speed.MilesPerHour = 1;

        Console.WriteLine("speed: {0} m/s is equal {1} km/h {2} mph",
            Math.Round(speed.MetersPerSecond, 4),
            Math.Round(speed.KilometersPerHour, 4),
            Math.Round(speed.MilesPerHour, 4));

        station.shutdown();

        return 0;
    }
}
