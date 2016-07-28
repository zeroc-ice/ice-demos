// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Demo;
using System;
using System.Collections.Generic;
using System.Reflection;

[assembly: CLSCompliant(true)]

[assembly: AssemblyTitle("IceInvokeClient")]
[assembly: AssemblyDescription("Ice invoke demo client")]
[assembly: AssemblyCompany("ZeroC, Inc.")]

public class Client
{
    private class ReadValueCallback
    {
        public void invoke(Ice.Value obj)
        {
            this.obj = obj;
        }

        internal Ice.Value obj;
    }

    public class App : Ice.Application
    {
        private static void menu()
        {
            Console.WriteLine(
                "usage:\n" +
                "1: print string\n" +
                "2: print string sequence\n" +
                "3: print dictionary\n" +
                "4: print enum\n" +
                "5: print struct\n" +
                "6: print struct sequence\n" +
                "7: print class\n" +
                "8: get values\n" +
                "9: throw exception\n" +
                "s: shutdown server\n" +
                "x: exit\n" +
                "?: help\n");
        }

        public override int run(string[] args)
        {
            if(args.Length > 0)
            {
                Console.Error.WriteLine(appName() + ": too many arguments");
                return 1;
            }

            var obj = communicator().propertyToProxy("Printer.Proxy");

            menu();

            string line = null;
            do
            {
                try
                {
                    Console.Write("==> ");
                    Console.Out.Flush();
                    line = Console.In.ReadLine();
                    if(line == null)
                    {
                        break;
                    }

                    byte[] outParams;

                    if(line.Equals("1"))
                    {
                        //
                        // Marshal the in parameter.
                        //
                        var outStream = new Ice.OutputStream(communicator());
                        outStream.startEncapsulation();
                        outStream.writeString("The streaming API works!");
                        outStream.endEncapsulation();

                        //
                        // Invoke operation.
                        //
                        if(!obj.ice_invoke("printString", Ice.OperationMode.Normal, outStream.finished(),
                                           out outParams))
                        {
                            Console.Error.WriteLine("Unknown user exception");
                        }
                    }
                    else if(line.Equals("2"))
                    {
                        //
                        // Marshal the in parameter.
                        //
                        var outStream = new Ice.OutputStream(communicator());
                        outStream.startEncapsulation();
                        string[] arr = { "The", "streaming", "API", "works!" };
                        StringSeqHelper.write(outStream, arr);
                        outStream.endEncapsulation();

                        //
                        // Invoke operation.
                        //
                        if(!obj.ice_invoke("printStringSequence", Ice.OperationMode.Normal, outStream.finished(),
                                           out outParams))
                        {
                            Console.Error.WriteLine("Unknown user exception");
                        }
                    }
                    else if(line.Equals("3"))
                    {
                        //
                        // Marshal the in parameter.
                        //
                        var outStream = new Ice.OutputStream(communicator());
                        outStream.startEncapsulation();
                        var dict = new Dictionary<string, string>()
                            {
                                { "The", "streaming" },
                                { "API", "works!"}
                            };
                        StringDictHelper.write(outStream, dict);
                        outStream.endEncapsulation();

                        //
                        // Invoke operation.
                        //
                        if(!obj.ice_invoke("printDictionary", Ice.OperationMode.Normal, outStream.finished(),
                                           out outParams))
                        {
                            Console.Error.WriteLine("Unknown user exception");
                        }
                    }
                    else if(line.Equals("4"))
                    {
                        //
                        // Marshal the in parameter.
                        //
                        var outStream = new Ice.OutputStream(communicator());
                        outStream.startEncapsulation();
                        ColorHelper.write(outStream, Color.green);
                        outStream.endEncapsulation();

                        //
                        // Invoke operation.
                        //
                        if(!obj.ice_invoke("printEnum", Ice.OperationMode.Normal, outStream.finished(), out outParams))
                        {
                            Console.Error.WriteLine("Unknown user exception");
                        }
                    }
                    else if(line.Equals("5"))
                    {
                        //
                        // Marshal the in parameter.
                        //
                        var outStream = new Ice.OutputStream(communicator());
                        outStream.startEncapsulation();
                        var s = new Structure("read", Color.red);
                        Structure.write(outStream, s);
                        outStream.endEncapsulation();

                        //
                        // Invoke operation.
                        //
                        if(!obj.ice_invoke("printStruct", Ice.OperationMode.Normal, outStream.finished(),
                                           out outParams))
                        {
                            Console.Error.WriteLine("Unknown user exception");
                        }
                    }
                    else if(line.Equals("6"))
                    {
                        //
                        // Marshal the in parameter.
                        //
                        var outStream = new Ice.OutputStream(communicator());
                        outStream.startEncapsulation();
                        Structure[] arr = 
                            {
                                new Structure("red", Color.red),
                                new Structure("green", Color.green),
                                new Structure("blue", Color.blue)
                            };
                        StructureSeqHelper.write(outStream, arr);
                        outStream.endEncapsulation();

                        //
                        // Invoke operation.
                        //
                        if(!obj.ice_invoke("printStructSequence", Ice.OperationMode.Normal, outStream.finished(),
                                           out outParams))
                        {
                            Console.Error.WriteLine("Unknown user exception");
                        }
                    }
                    else if(line.Equals("7"))
                    {
                        //
                        // Marshal the in parameter.
                        //
                        Ice.OutputStream outStream = new Ice.OutputStream(communicator());
                        outStream.startEncapsulation();
                        var c = new C(new Structure("blue", Color.blue));
                        outStream.writeValue(c);
                        outStream.writePendingValues();
                        outStream.endEncapsulation();

                        //
                        // Invoke operation.
                        //
                        if(!obj.ice_invoke("printClass", Ice.OperationMode.Normal, outStream.finished(), out outParams))
                        {
                            Console.Error.WriteLine("Unknown user exception");
                        }
                    }
                    else if(line.Equals("8"))
                    {
                        //
                        // Invoke operation.
                        //
                        if(!obj.ice_invoke("getValues", Ice.OperationMode.Normal, null, out outParams))
                        {
                            Console.Error.WriteLine("Unknown user exception");
                            continue;
                        }

                        //
                        // Unmarshal the results.
                        //
                        var inStream = new Ice.InputStream(communicator(), outParams);
                        inStream.startEncapsulation();
                        var cb = new ReadValueCallback();
                        inStream.readValue(cb.invoke);
                        var str = inStream.readString();
                        inStream.readPendingValues();
                        var c = cb.obj as C;
                        Console.Error.WriteLine("Got string `" + str + "' and class: s.name=" + c.s.name +
                                                ", s.value=" + c.s.value);
                    }
                    else if(line.Equals("9"))
                    {
                        //
                        // Invoke operation.
                        //
                        if(obj.ice_invoke("throwPrintFailure", Ice.OperationMode.Normal, null, out outParams))
                        {
                            Console.Error.WriteLine("Expected exception");
                            continue;
                        }

                        var inStream = new Ice.InputStream(communicator(), outParams);
                        inStream.startEncapsulation();
                        try
                        {
                            inStream.throwException();
                        }
                        catch(PrintFailure)
                        {
                            // Expected.
                        }
                        catch(Ice.UserException)
                        {
                            Console.Error.WriteLine("Unknown user exception");
                        }
                        inStream.endEncapsulation();
                    }
                    else if(line.Equals("s"))
                    {
                        obj.ice_invoke("shutdown", Ice.OperationMode.Normal, null, out outParams);
                    }
                    else if(line.Equals("x"))
                    {
                        // Nothing to do.
                    }
                    else if(line.Equals("?"))
                    {
                        menu();
                    }
                    else
                    {
                        Console.Error.WriteLine("unknown command `" + line + "'");
                        menu();
                    }
                }
                catch(Ice.LocalException ex)
                {
                    Console.Error.WriteLine(ex);
                }
            }
            while(!line.Equals("x"));

            return 0;
        }
    }

    public static int Main(string[] args)
    {
        var app = new App();
        return app.main(args, "config.client");
    }
}
