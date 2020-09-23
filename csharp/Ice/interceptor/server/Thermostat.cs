// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using ZeroC.Ice;

namespace Demo
{
    class Thermostat : IThermostat
    {
        // Temperature in Celsius.
        private float _temperature = 23.5f;

        public float GetTemp(Current current)
        {
            lock (this)
            {
                return _temperature;
            }
        }

        public void SetTemp(float temp, Current current)
        {
            lock (this)
            {
                _temperature = temp;
            }
        }

        public void Shutdown(Current current)
        {
            Console.Out.WriteLine("Shutting down...");
            current.Adapter.Communicator.DisposeAsync();
        }
    }
}
