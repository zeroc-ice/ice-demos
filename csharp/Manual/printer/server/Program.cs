// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using System.Threading;
using ZeroC.Ice;

using var communicator = new Communicator(ref args);
ObjectAdapter adapter = communicator.CreateObjectAdapterWithEndpoints("SimplePrinterAdapter",
                                                                      "ice+tcp://localhost:10000");
adapter.Add("SimplePrinter", new Demo.Printer());
await adapter.ActivateAsync();
communicator.WaitForShutdown();

namespace Demo
{
    public class Printer : Demo.IPrinter
    {
        public void PrintString(string s, Current current, CancellationToken cancel) => Console.WriteLine(s);
    }
}
