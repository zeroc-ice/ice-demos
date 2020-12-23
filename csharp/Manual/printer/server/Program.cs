// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using System.Threading;
using ZeroC.Ice;

await using var communicator = new Communicator(ref args);
await communicator.ActivateAsync();
ObjectAdapter adapter = communicator.CreateObjectAdapterWithEndpoints("SimplePrinterAdapter",
                                                                      "ice+tcp://localhost:10000");
adapter.Add("SimplePrinter", new Demo.Printer());
await adapter.ActivateAsync();
await communicator.WaitForShutdownAsync();

namespace Demo
{
    public class Printer : Demo.IPrinter
    {
        public void PrintString(string s, Current current, CancellationToken cancel) => Console.WriteLine(s);
    }
}
