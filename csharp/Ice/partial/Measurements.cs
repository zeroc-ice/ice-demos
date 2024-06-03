// Copyright (c) ZeroC, Inc.

namespace Demo
{
    /// <summary> Because the Distance generated struct is defined as a partial type,
    /// we can manually add some extra methods that are convenient for our
    /// application.
    /// </summary>
    public partial record struct Distance
    {
        /// <summary> Gets or sets the distance in yards.
        /// </summary>
        public double Yards
        {
            readonly get
            {
                return value * YardConversionFactor;
            }

            set
            {
                this.value = value / YardConversionFactor;
            }
        }

        /// <summary> Gets or sets the distance in meters.
        /// </summary>
        public double Meters
        {
            readonly get
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

    /// <summary> Because the Temperature generated class is defined as a partial type,
    /// we can manually add some extra methods that are convenient for our
    /// application.
    /// </summary>
    public partial class Speed
    {
        /// <summary> Gets or sets the speed in miles per hour.
        /// </summary>
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

        /// <summary> Gets or sets the speed in kilometers per hour.
        /// </summary>
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

        /// <summary> Gets or sets the speed in meters per second.
        /// </summary>
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
