// Copyright (c) ZeroC, Inc. All rights reserved.

namespace Demo
{
    //
    // Because the Distance generated struct is defined as a partial type,
    // we can manually add some extra methods that are covenient for our
    // application.
    //
    partial struct Distance
    {
        public double Yards
        {
            get
            {
                return value * YardConversionFactor;
            }
            set
            {
                this.value = value / YardConversionFactor;
            }
        }

        public double Meters
        {
            get
            {
                return value;
            }
            set
            {
                this.value = value;
            }
        }

        private const double YardConversionFactor = 1.0936;
    }
    //
    // Because the Speed generated class is defined as a partial type,
    // we can manually add some extra methods that are covenient for our
    // application.
    //
    partial class Speed
    {
        public double MilesPerHour
        {
            get
            {
                return value * MilesPerHourConversionFactor;
            }
            set
            {
                this.value = value / MilesPerHourConversionFactor;
            }
        }

        public double KilometersPerHour
        {
            get
            {
                return value * KilometersPerHourConversionFactor;
            }
            set
            {
                this.value = value / KilometersPerHourConversionFactor;
            }
        }

        public double MetersPerSecond
        {
            get
            {
                return value;
            }
            set
            {
                this.value = value;
            }
        }

        private const double MilesPerHourConversionFactor = 2.2369;
        private const double KilometersPerHourConversionFactor = 3.6;
    }
}
