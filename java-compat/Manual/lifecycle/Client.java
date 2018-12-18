// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Filesystem.*;

public class Client
{
    public static void main(String[] args)
    {
        int status = 0;
        //
        // Try with resources block - communicator is automatically destroyed
        // at the end of this try block
        //
        try(Ice.Communicator communicator = Ice.Util.initialize(args))
        {
            //
            // Create a proxy for the root directory
            //
            Ice.ObjectPrx base = communicator.stringToProxy("RootDir:default -h localhost -p 10000");

            //
            // Down-cast the proxy to a Directory proxy.
            //
            DirectoryPrx rootDir = DirectoryPrxHelper.checkedCast(base);
            if(rootDir == null)
            {
                throw new Error("Invalid proxy");
            }

            Parser p = new Parser(rootDir);
            status = p.parse();
        }
        System.exit(status);
    }

    private static class Error extends RuntimeException
    {
        public Error(String msg)
        {
            super(msg);
        }
    }
}
