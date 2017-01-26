// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Filesystem.*;

public class Client extends com.zeroc.Ice.Application
{
    @Override
    public int run(String[] args)
    {
        //
        // Terminate cleanly on receipt of a signal.
        //
        shutdownOnInterrupt();

        //
        // Create a proxy for the root directory
        //
        com.zeroc.Ice.ObjectPrx base = communicator().stringToProxy("RootDir:default -h localhost -p 10000");

        //
        // Down-cast the proxy to a Directory proxy.
        //
        DirectoryPrx rootDir = DirectoryPrx.checkedCast(base);
        if(rootDir == null)
        {
            throw new Error("Invalid proxy");
        }

        Parser p = new Parser(rootDir);
        return p.parse();
    }

    static public void main(String[] args)
    {
        Client app = new Client();
        app.main("demo.book.lifecycle.Client", args);
    }

    static private class Error extends RuntimeException
    {
        public Error(String msg)
        {
            super(msg);
        }
    }
}
