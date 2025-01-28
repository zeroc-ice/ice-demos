// Copyright (c) ZeroC, Inc.

import com.zeroc.Ice.*;

public class Server
{
    public static void main(String[] args)
    {
        try(Communicator communicator = Util.initialize(args))
        {
            // Shut down the communicator on Ctrl+C
            Runtime.getRuntime().addShutdownHook(new Thread(() -> communicator.shutdown()));

            ObjectAdapter adapter =
                communicator.createObjectAdapterWithEndpoints("Hello", "default -h localhost -p 10000");
            adapter.add(new Printer(), Util.stringToIdentity("hello"));
            adapter.activate();

            communicator.waitForShutdown();
        }
    }
}
