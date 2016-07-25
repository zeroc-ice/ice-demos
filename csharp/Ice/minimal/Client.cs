// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using System;
using Demo;

public class Client
{
    public static int Main(string[] args)
    {
        try
        {
            using(var communicator = Ice.Util.initialize(ref args))
            {
                var hello = HelloPrxHelper.checkedCast(
                    communicator.stringToProxy("hello:default -h localhost -p 10000"));
                hello.sayHello();
            }
            return 0;
        }
        catch(Exception ex)
        {
            Console.Error.WriteLine(ex);
            return 1;
        }
    }
}
