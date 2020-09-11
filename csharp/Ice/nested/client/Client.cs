//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

using Demo;
using System;
using System.Configuration;
using System.Threading;
using ZeroC.Ice;


// The new communicator is automatically destroyed (disposed) at the end of the using statement
using var communicator = new Communicator(ref args, ConfigurationManager.AppSettings);

// The communicator initialization removes all Ice-related arguments from args
if(args.Length > 0)
{
    throw new ArgumentException("too many arguments");
}


INestedPrx nested = communicator.GetPropertyAsProxy("Nested.Proxy", INestedPrx.Factory) ??
    throw new ArgumentException("invalid proxy");

// Ensure the invocation times out if the nesting level is too high and there are no more threads in the thread pool to
// dispatch the call.

ObjectAdapter adapter = communicator.CreateObjectAdapter("Nested.Client");
INestedPrx self = adapter.CreateProxy("nestedClient", INestedPrx.Factory);
adapter.Add("nestedClient", new Nested(self));
adapter.Activate();

Console.Out.WriteLine("Note: The maximum nesting level is sz * 2, with sz being");
Console.Out.WriteLine("the maximum number of threads in the server thread pool. If");
Console.Out.WriteLine("you specify a value higher than that, the application will");
Console.Out.WriteLine("block or timeout.");
Console.Out.WriteLine();

string? s = null;
do
{
    try
    {
        Console.Out.Write("enter nesting level or 'x' for exit: ");
        Console.Out.Flush();
        s = Console.In.ReadLine();
        if(s == null)
        {
            break;
        }
        
        if (int.TryParse(s, out int level))
        {
            if (level > 0)
            {
                using var cancel = new CancellationTokenSource(5000);
                nested.NestedCall(level, self, cancel: cancel.Token);
            }
        }
    }
    catch(Exception ex)
    {
        Console.Error.WriteLine(ex);
    }
}
while(!s.Equals("x"));
