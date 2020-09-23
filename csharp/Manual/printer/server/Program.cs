// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using ZeroC.Ice;

using var communicator = new Communicator(ref args);
ObjectAdapter adapter = communicator.CreateObjectAdapterWithEndpoints("SimplePrinterAdapter",
                                                                      "ice+tcp://localhost:10000");
adapter.Add("SimplePrinter", new Demo.Printer());
adapter.Activate();
communicator.WaitForShutdown();

namespace Demo
{
    public class Printer : Demo.IPrinter
    {
        public void PrintString(string s, Current current) => Console.WriteLine(s);
    }
}
