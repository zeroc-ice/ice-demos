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
        try
        {
            //
            // The new communicator is automatically destroyed (disposed) at the end of the
            // using statement
            //
            using(var communicator = Ice.Util.initialize(ref args))
            {
                //
                // Create a proxy for the root directory
                //
                var @base = communicator.stringToProxy("RootDir:default -h localhost -p 10000");

                //
                // Down-cast the proxy to a Directory proxy.
                //
                var rootDir = DirectoryPrxHelper.checkedCast(@base);
                if(rootDir == null)
                {
                    throw new Error("Invalid proxy");
                }

                var p = new Parser(rootDir);
                return p.parse();
            }
        }
        catch(Exception ex)
        {
            Console.Error.WriteLine(ex);
            return 1;
        }
    }

    private class Error : SystemException
    {
        public Error(String msg)
            : base(msg)
        {
        }
    }
}
