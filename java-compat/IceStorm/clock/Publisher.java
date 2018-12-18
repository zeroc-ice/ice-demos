// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

public class Publisher
{
    static class ShutdownHook extends Thread
    {
        @Override
        public void
        run()
        {
            _communicator.destroy();
        }

        ShutdownHook(Ice.Communicator communicator)
        {
            _communicator = communicator;
        }

        private final Ice.Communicator _communicator;
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
        try(Ice.Communicator communicator = Ice.Util.initialize(argsHolder, "config.pub"))
        {
            //
            // Install shutdown hook to (also) destroy communicator during JVM shutdown.
            // This ensures the communicator gets destroyed when the user interrupts the application with Ctrl-C.
            //
            Runtime.getRuntime().addShutdownHook(new ShutdownHook(communicator));

            status = run(communicator, argsHolder.value);
        }
        System.exit(status);
    }

    public static void
    usage()
    {
        System.out.println("Usage: [--datagram|--twoway|--oneway] [topic]");
    }

    private static int
    run(Ice.Communicator communicator, String[] args)
    {
        String option = "None";
        String topicName = "time";
        int i;

        for(i = 0; i < args.length; ++i)
        {
            String oldoption = option;
            if(args[i].equals("--datagram"))
            {
                option = "Datagram";
            }
            else if(args[i].equals("--twoway"))
            {
                option = "Twoway";
            }
            else if(args[i].equals("--oneway"))
            {
                option = "Oneway";
            }
            else if(args[i].startsWith("--"))
            {
                usage();
                return 1;
            }
            else
            {
                topicName = args[i++];
                break;
            }

            if(!oldoption.equals(option) && !oldoption.equals("None"))
            {
                usage();
                return 1;
            }
        }

        if(i != args.length)
        {
            usage();
            return 1;
        }

        IceStorm.TopicManagerPrx manager = IceStorm.TopicManagerPrxHelper.checkedCast(
            communicator.propertyToProxy("TopicManager.Proxy"));
        if(manager == null)
        {
            System.err.println("invalid proxy");
            return 1;
        }

        //
        // Retrieve the topic.
        //
        IceStorm.TopicPrx topic;
        try
        {
            topic = manager.retrieve(topicName);
        }
        catch(IceStorm.NoSuchTopic e)
        {
            try
            {
                topic = manager.create(topicName);
            }
            catch(IceStorm.TopicExists ex)
            {
                System.err.println("temporary failure, try again.");
                return 1;
            }
        }

        //
        // Get the topic's publisher object, and create a Clock proxy with
        // the mode specified as an argument of this application.
        //
        Ice.ObjectPrx publisher = topic.getPublisher();
        if(option.equals("Datagram"))
        {
            publisher = publisher.ice_datagram();
        }
        else if(option.equals("Twoway"))
        {
            // Do nothing.
        }
        else // if(oneway)
        {
            publisher = publisher.ice_oneway();
        }
        ClockPrx clock = ClockPrxHelper.uncheckedCast(publisher);

        System.out.println("publishing tick events. Press ^C to terminate the application.");
        try
        {
            java.text.SimpleDateFormat date = new java.text.SimpleDateFormat("MM/dd/yy HH:mm:ss:SSS");
            while(true)
            {

                clock.tick(date.format(new java.util.Date()));

                try
                {
                    Thread.currentThread();
                    Thread.sleep(1000);
                }
                catch(java.lang.InterruptedException e)
                {
                }
            }
        }
        catch(Ice.CommunicatorDestroyedException ex)
        {
            // Ctrl-C triggered shutdown hook, which destroyed communicator - we're terminating
        }

        return 0;
    }
}
