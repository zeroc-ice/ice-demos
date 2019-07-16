//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

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
                return value * YardConversionFactor.value;
            }
            set
            {
                this.value = value / YardConversionFactor.value;
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
                return value * MilesPerHourConversionFactor.value;
            }
            set
            {
                this.value = value / MilesPerHourConversionFactor.value;
            }
        }

        public double KilometresPerHour
        {
            get
            {
                return value * KilometresPerHourConversionFactor.value;
            }
            set
            {
                this.value = value / KilometresPerHourConversionFactor.value;
            }
        }

        public double MetrePerSecond
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
    }
}
