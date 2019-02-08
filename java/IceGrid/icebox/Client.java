//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.IceGrid.icebox.Demo.*;

public class Client
{
    public static void main(String[] args)
    {
        int status = 0;
        java.util.List<String> extraArgs = new java.util.ArrayList<>();

        //
        // Try with resources block - communicator is automatically destroyed
        // at the end of this try block
        //
        try(com.zeroc.Ice.Communicator communicator = com.zeroc.Ice.Util.initialize(args, "config.client", extraArgs))
        {
            communicator.getProperties().setProperty("Ice.Default.Package", "com.zeroc.demos.IceGrid.icebox");
            //
            // Install shutdown hook to (also) destroy communicator during JVM shutdown.
            // This ensures the communicator gets destroyed when the user interrupts the application with Ctrl-C.
            //
            Runtime.getRuntime().addShutdownHook(new Thread(() -> communicator.destroy()));

            if(!extraArgs.isEmpty())
            {
                System.err.println("too many arguments");
                status = 1;
            }
            else
            {
                HelloPrx hello = HelloPrx.uncheckedCast(communicator.propertyToProxy("Hello.Proxy"));

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
