// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

public class Client
{
    public static void main(String[] args)
    {
        try
        {
            com.zeroc.Ice.Util.InitializeResult r = com.zeroc.Ice.Util.initialize(args);
            HelloPrx hello = HelloPrx.checkedCast(
                r.communicator.stringToProxy("hello:default -h localhost -p 10000"));
            hello.sayHello();
            r.communicator.destroy();
        }
        catch(com.zeroc.Ice.LocalException ex)
        {
            ex.printStackTrace();
            System.exit(1);
        }
    }
}
