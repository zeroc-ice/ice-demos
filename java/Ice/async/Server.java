//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

public class Server
{
    public static void main(String[] args)
    {
        int status = 0;
        java.util.List<String> extraArgs = new java.util.ArrayList<String>();

        //
        // Try with resources block - communicator is automatically destroyed
        // at the end of this try block
        //
        try(com.zeroc.Ice.Communicator communicator = com.zeroc.Ice.Util.initialize(args, "config.server", extraArgs))
        {
            communicator.getProperties().setProperty("Ice.Default.Package", "com.zeroc.demos.Ice.async");
            final WorkQueue workQueue = new WorkQueue();

            //
            // Install shutdown hook to destroy workqueue and communicator during JVM shutdown
            //
            Runtime.getRuntime().addShutdownHook(new Thread(() ->
            {
                workQueue._destroy();
                try
                {
                    workQueue.join();
                }
                catch(InterruptedException e)
                {
                }
                finally
                {
                    communicator.destroy();
                }
            }));

            if(!extraArgs.isEmpty())
            {
                System.err.println("too many arguments");
                status = 1;
            }
            else
            {
                com.zeroc.Ice.ObjectAdapter adapter = communicator.createObjectAdapter("Hello");
                workQueue.start();
                adapter.add(new HelloI(workQueue), com.zeroc.Ice.Util.stringToIdentity("hello"));
                adapter.activate();
                communicator.waitForShutdown();

                workQueue._destroy();
                try
                {
                    workQueue.join();
                }
                catch(InterruptedException e)
                {
                }
            }
        }

        System.exit(status);
    }
}
