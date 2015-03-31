// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Demo;

public class Client
{
    public static int Main(string[] args)
    {
        try
        {
            using(Ice.Communicator communicator = Ice.Util.initialize(ref args))
            {
                HelloPrx hello = HelloPrxHelper.checkedCast(
                    communicator.stringToProxy("hello:default -h localhost -p 10000"));
                hello.sayHello();
            }
            return 0;
        }
        catch(System.Exception ex)
        {
            System.Console.Error.WriteLine(ex);
            return 1;
        }
    }
}
