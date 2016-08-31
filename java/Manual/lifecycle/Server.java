// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import FilesystemI.*;

class Server extends com.zeroc.Ice.Application
{
    @Override
    public int run(String[] args)
    {
        //
        // Terminate cleanly on receipt of a signal.
        //
        shutdownOnInterrupt();

        //
        // Create an object adapter
        //
        com.zeroc.Ice.ObjectAdapter adapter = communicator().createObjectAdapterWithEndpoints(
            "LifecycleFilesystem", "default -h localhost -p 10000");

        //
        // Create the root directory.
        //
        DirectoryI root = new DirectoryI();
        com.zeroc.Ice.Identity id = new com.zeroc.Ice.Identity();
        id.name = "RootDir";
        adapter.add(root, id);

        //
        // All objects are created, allow client requests now.
        //
        adapter.activate();

        //
        // Wait until we are done.
        //
        communicator().waitForShutdown();
        if(interrupted())
        {
            System.err.println(appName() + ": received signal, shutting down");
        }

        return 0;
    }

    static public void main(String[] args)
    {
        Server app = new Server();
        app.main("demo.book.lifecycle.Server", args);
    }
}
