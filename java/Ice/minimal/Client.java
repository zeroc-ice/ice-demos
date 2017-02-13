// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

public class Client
{
    public static void main(String[] args) throws Exception
    {
        try(com.zeroc.Ice.Communicator communicator = com.zeroc.Ice.Util.initialize(args))
        {
            HelloPrx hello = HelloPrx.checkedCast(communicator.stringToProxy("hello:default -h localhost -p 10000"));
            hello.sayHello();
            communicator.destroy();
        }
    }
}
