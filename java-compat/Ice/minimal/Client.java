// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

public class Client
{
    public static void
    main(String[] args)
    {
        try(Ice.Communicator com = Ice.Util.initialize(args))
        {
            HelloPrx hello = HelloPrxHelper.checkedCast(com.stringToProxy("hello:default -h localhost -p 10000"));
            hello.sayHello();
        }
    }
}
