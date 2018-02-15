// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

public class Client
{
    static class ShutdownHook implements Runnable
    {
        private Ice.Communicator communicator;

        ShutdownHook(Ice.Communicator communicator)
        {
            this.communicator = communicator;
        }

        @Override
        public void
        run()
        {
            //
            // Destroy communicator to abandon ongoing remote calls
            // calling destroy multiple times is no-op
            //
            communicator.destroy();
        }
    }

    public static void
    main(String[] args)
    {
        int status = 0;
        Ice.StringSeqHolder argsHolder = new Ice.StringSeqHolder(args);

        //
        // Try with resources block - communicator is automatically destroyed
        // at the end of this try block
        //
        try(Ice.Communicator communicator = Ice.Util.initialize(argsHolder, "config.client"))
        {
            //
            // Install shutdown hook for user interrupt like Ctrl-C
            //
            Runtime.getRuntime().addShutdownHook(new Thread(new ShutdownHook(communicator)));

            if(argsHolder.value.length > 0)
            {
                System.err.println("too many arguments");
                status = 1;
            }
            else
            {
                HelloPrx hello = HelloPrxHelper.uncheckedCast(communicator.propertyToProxy("Hello.Proxy"));

                if(hello == null)
                {
                    System.err.println("Hello.Proxy not set");
                    status = 1;
                }
                else
                {
                    hello.sayHello();
                }
            }
        }

        System.exit(status);
    }
}
