// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

public class Client extends com.zeroc.Ice.Application
{
    @Override
    public int run(String[] args)
    {
        if(args.length > 0)
        {
            System.err.println(appName() + ": too many arguments");
            return 1;
        }

        HelloPrx hello = HelloPrx.uncheckedCast(communicator().propertyToProxy("Hello.Proxy"));

        if(hello == null)
        {
            System.err.println(appName() + ": Hello.Proxy not set");
            return 1;
        }

        hello.sayHello();

        return 0;
    }

    public static void main(String[] args)
    {
        Client app = new Client();
        int status = app.main("Client", args, "config.client");
        System.exit(status);
    }
}
