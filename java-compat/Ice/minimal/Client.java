// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

public class Client
{
    public static void
    main(String[] args)
    {
        try
        {
            Ice.Communicator communicator = Ice.Util.initialize(args);
            HelloPrx hello = HelloPrxHelper.checkedCast(
                communicator.stringToProxy("hello:default -h localhost -p 10000"));
            hello.sayHello();
            communicator.destroy();
        }
        catch(Ice.LocalException ex)
        {
            ex.printStackTrace();
            System.exit(1);
        }
    }
}
