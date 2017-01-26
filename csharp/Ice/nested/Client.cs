// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Demo;
using System;
using System.Reflection;

[assembly: CLSCompliant(true)]

[assembly: AssemblyTitle("IceNestedClient")]
[assembly: AssemblyDescription("Ice nested demo client")]
[assembly: AssemblyCompany("ZeroC, Inc.")]

public class Client
{
    public class App : Ice.Application
    {
        public override int run(string[] args)
        {
            if(args.Length > 0)
            {
                Console.Error.WriteLine(appName() + ": too many arguments");
                return 1;
            }

            var nested = NestedPrxHelper.checkedCast(communicator().propertyToProxy("Nested.Proxy"));
            if(nested == null)
            {
                Console.Error.WriteLine("invalid proxy");
                return 1;
            }

            //
            // Ensure the invocation times out if the nesting level is too
            // high and there are no more threads in the thread pool to
            // dispatch the call.
            //
            nested = (NestedPrx)nested.ice_invocationTimeout(5000);

            var adapter = communicator().createObjectAdapter("Nested.Client");
            var self = 
                NestedPrxHelper.uncheckedCast(adapter.createProxy(Ice.Util.stringToIdentity("nestedClient")));
            adapter.add(new NestedI(self), Ice.Util.stringToIdentity("nestedClient"));
            adapter.activate();
            
            Console.Out.WriteLine("Note: The maximum nesting level is sz * 2, with sz being");
            Console.Out.WriteLine("the maximum number of threads in the server thread pool. If");
            Console.Out.WriteLine("you specify a value higher than that, the application will");
            Console.Out.WriteLine("block or timeout.");
            Console.Out.WriteLine();
            
            string s = null;
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
                    var level = int.Parse(s);
                    if(level > 0)
                    {
                        nested.nestedCall(level, self);
                    }
                }
                catch(FormatException)
                {
                }
                catch(Exception ex)
                {
                    Console.Error.WriteLine(ex);
                }
            }
            while(!s.Equals("x"));
            
            return 0;
        }
    }

    public static int Main(string[] args)
    {
        var app = new App();
        return app.main(args, "config.client");
    }
}
