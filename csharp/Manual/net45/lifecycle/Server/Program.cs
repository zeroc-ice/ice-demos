// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using FilesystemI;
using System;

public class Program
{
    public static int Main(string[] args)
    {
        int status = 0;

        try
        {
            //
            // using statement - communicator is automatically destroyed
            // at the end of this statement
            //
            using(var communicator = Ice.Util.initialize(ref args))
            {
                //
                // Destroy the communicator on Ctrl+C or Ctrl+Break
                //
                Console.CancelKeyPress += (sender, eventArgs) => communicator.destroy();

                //
                // Create an object adapter
                //
                var adapter = communicator.createObjectAdapterWithEndpoints(
                    "LifecycleFilesystem", "default -h localhost -p 10000");

                //
                // Create the root directory.
                //
                var root = new DirectoryI();
                var id = new Ice.Identity();
                id.name = "RootDir";
                adapter.add(root, id);

                //
                // All objects are created, allow client requests now.
                //
                adapter.activate();

                //
                // Wait until we are done.
                //
                communicator.waitForShutdown();
            }
        }
        catch(Exception ex)
        {
            Console.Error.WriteLine(ex);
            status = 1;
        }

        return status;
    }
}
