// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Demo;
using System;
using System.Diagnostics;
using System.Globalization;

public class Publisher
{
    public static int Main(string[] args)
    {
        int status = 0;

        try
        {
            //
            // using statement - communicator is automatically destroyed
            // at the end of this statement
            //
            using(var communicator = Ice.Util.initialize(ref args, "config.pub"))
            {
                //
                // Destroy the communicator on Ctrl+C or Ctrl+Break
                //
                Console.CancelKeyPress += (sender, eventArgs) => communicator.destroy();

                status = run(communicator, args);
            }
        }
        catch(Exception ex)
        {
            Console.Error.WriteLine(ex);
            status = 1;
        }

        return status;
    }

    private static int run(Ice.Communicator communicator, string[] args)
    {
        string option = "None";
        string topicName = "time";
        int i;

        for(i = 0; i < args.Length; ++i)
        {
            String oldoption = option;
            if(args[i].Equals("--datagram"))
            {
                option = "Datagram";
            }
            else if(args[i].Equals("--twoway"))
            {
                option = "Twoway";
            }
            else if(args[i].Equals("--oneway"))
            {
                option = "Oneway";
            }
            else if(args[i].StartsWith("--"))
            {
                usage();
                return 1;
            }
            else
            {
                topicName = args[i++];
                break;
            }

            if(!oldoption.Equals(option) && !oldoption.Equals("None"))
            {
                usage();
                return 1;
            }
        }

        if(i != args.Length)
        {
            usage();
            return 1;
        }

        IceStorm.TopicManagerPrx manager = IceStorm.TopicManagerPrxHelper.checkedCast(
            communicator.propertyToProxy("TopicManager.Proxy"));
        if(manager == null)
        {
            Console.WriteLine("invalid proxy");
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
        catch(IceStorm.NoSuchTopic)
        {
            try
            {
                topic = manager.create(topicName);
            }
            catch(IceStorm.TopicExists)
            {
                Console.WriteLine("temporary error. try again.");
                return 1;
            }
        }

        //
        // Get the topic's publisher object, and create a Clock proxy with
        // the mode specified as an argument of this application.
        //
        Ice.ObjectPrx publisher = topic.getPublisher();
        if(option.Equals("Datagram"))
        {
            publisher = publisher.ice_datagram();
        }
        else if(option.Equals("Twoway"))
        {
            // Do nothing.
        }
        else // if(oneway)
        {
            publisher = publisher.ice_oneway();
        }
        ClockPrx clock = ClockPrxHelper.uncheckedCast(publisher);

        Console.WriteLine("publishing tick events. Press ^C to terminate the application.");
        try
        {
            while(true)
            {
                clock.tick(System.DateTime.Now.ToString("G", DateTimeFormatInfo.InvariantInfo));
                System.Threading.Thread.Sleep(1000);
            }
        }
        catch(Ice.CommunicatorDestroyedException)
        {
            // Ignore
        }

        return 0;
    }

    private static void usage()
    {
        Console.WriteLine("Usage: [--datagram|--twoway|--oneway] [topic]");
    }
}
