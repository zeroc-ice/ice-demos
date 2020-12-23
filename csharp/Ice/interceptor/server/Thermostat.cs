// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using System.Threading;
using ZeroC.Ice;

namespace Demo
{
    class Thermostat : IThermostat
    {
        // Temperature in Celsius.
        private float _temperature = 23.5f;

        public float GetTemp(Current current, CancellationToken cancel)
        {
            lock (this)
            {
                return _temperature;
            }
        }

        public void SetTemp(float temp, Current current, CancellationToken cancel)
        {
            lock (this)
            {
                _temperature = temp;
            }
        }

        public void Shutdown(Current current, CancellationToken cancel)
        {
            Console.Out.WriteLine("Shutting down...");
            current.Adapter.Communicator.ShutdownAsync();
        }
    }
}
