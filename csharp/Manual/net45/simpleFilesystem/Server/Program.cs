// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Filesystem;
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

                status = run(communicator);
            }
        }
        catch(Exception ex)
        {
            Console.Error.WriteLine(ex);
            status = 1;
        }

        return status;
    }

    private static int run(Ice.Communicator communicator)
    {
        //
        // Create an object adapter.
        //
        var adapter =
            communicator.createObjectAdapterWithEndpoints("SimpleFilesystem", "default -h localhost -p 10000");

        //
        // Create the root directory (with name "/" and no parent)
        //
        var root = new DirectoryI(communicator, "/", null);
        root.activate(adapter);

        //
        // Create a file called "README" in the root directory
        //
        var file = new FileI(communicator, "README", root);
        var text = new string[]{ "This file system contains a collection of poetry." };
        try
        {
            file.write(text);
        }
        catch(GenericError e)
        {
            Console.Error.WriteLine(e.reason);
        }
        file.activate(adapter);

        //
        // Create a directory called "Coleridge" in the root directory
        //
        var coleridge = new DirectoryI(communicator, "Coleridge", root);
        coleridge.activate(adapter);

        //
        // Create a file called "Kubla_Khan" in the Coleridge directory
        //
        file = new FileI(communicator, "Kubla_Khan", coleridge);
        text = new string[]{ "In Xanadu did Kubla Khan",
                             "A stately pleasure-dome decree:",
                             "Where Alph, the sacred river, ran",
                             "Through caverns measureless to man",
                             "Down to a sunless sea." };
        try
        {
            file.write(text);
        }
        catch(GenericError e)
        {
            Console.Error.WriteLine(e.reason);
        }
        file.activate(adapter);

        //
        // All objects are created, allow client requests now
        //
        adapter.activate();

        //
        // Wait until we are done
        //
        communicator.waitForShutdown();

        return 0;
    }
}
