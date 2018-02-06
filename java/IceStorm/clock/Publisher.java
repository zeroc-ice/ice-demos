// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

public class Publisher
{
    public static void main(String[] args)
    {
        int status = 0;
        java.util.List<String> extraArgs = new java.util.ArrayList<String>();

        //
        // try with resources block - communicator is automatically destroyed
        // at the end of this try block
        //
        System.out.println("hello");
        try(com.zeroc.Ice.Communicator communicator = com.zeroc.Ice.Util.initialize(args, "config.pub", extraArgs))
        {
            System.out.print("things");
            Runtime.getRuntime().addShutdownHook(new Thread(() ->
            {
                communicator.shutdown();
            }));

            status = run(communicator, extraArgs.toArray(new String[extraArgs.size()]));
        }

        System.exit(status);
    }

    public static void usage()
    {
        System.out.println("Usage: [--datagram|--twoway|--oneway] [topic]");
    }

    private static int run(com.zeroc.Ice.Communicator communicator, String[] args)
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

        com.zeroc.IceStorm.TopicManagerPrx manager = com.zeroc.IceStorm.TopicManagerPrx.checkedCast(
            communicator.propertyToProxy("TopicManager.Proxy"));
        if(manager == null)
        {
            System.err.println("invalid proxy");
            return 1;
        }

        //
        // Retrieve the topic.
        //
        com.zeroc.IceStorm.TopicPrx topic;
        try
        {
            topic = manager.retrieve(topicName);
        }
        catch(com.zeroc.IceStorm.NoSuchTopic e)
        {
            try
            {
                topic = manager.create(topicName);
            }
            catch(com.zeroc.IceStorm.TopicExists ex)
            {
                System.err.println("temporary failure, try again.");
                return 1;
            }
        }

        //
        // Get the topic's publisher object, and create a Clock proxy with
        // the mode specified as an argument of this application.
        //
        com.zeroc.Ice.ObjectPrx publisher = topic.getPublisher();
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
        ClockPrx clock = ClockPrx.uncheckedCast(publisher);

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
        catch(com.zeroc.Ice.CommunicatorDestroyedException ex)
        {
            // Ignore
        }

        return 0;
    }
}
