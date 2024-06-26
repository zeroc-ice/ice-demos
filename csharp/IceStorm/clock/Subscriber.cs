// Copyright (c) ZeroC, Inc.

using Demo;
using System.Diagnostics;

internal class Subscriber
{
    internal class ClockI : ClockDisp_
    {
        public override void tick(string date, Ice.Current current)
        {
            Console.WriteLine(date);
        }
    }

    public static int Main(string[] args)
    {
        int status = 0;

        try
        {
            // using statement - communicator is automatically destroyed
            // at the end of this statement
            using var communicator = Ice.Util.initialize(ref args, "config.sub");
            // Shutdown the communicator on Ctrl+C or Ctrl+Break
            // (always use Cancel = true with shutdown)
            Console.CancelKeyPress += (sender, eventArgs) =>
            {
                eventArgs.Cancel = true;
                communicator.shutdown();
            };

            status = Run(communicator, args);
        }
        catch (Exception ex)
        {
            Console.Error.WriteLine(ex);
            status = 1;
        }

        return status;
    }

    private static int Run(Ice.Communicator communicator, string[] args)
    {
        args = communicator.getProperties().parseCommandLineOptions("Clock", args);

        string topicName = "time";
        string option = "None";
        bool batch = false;
        string id = null;
        string retryCount = null;
        int i;
        for (i = 0; i < args.Length; ++i)
        {
            string oldoption = option;
            if (args[i].Equals("--datagram"))
            {
                option = "Datagram";
            }
            else if (args[i].Equals("--twoway"))
            {
                option = "Twoway";
            }
            else if (args[i].Equals("--ordered"))
            {
                option = "Ordered";
            }
            else if (args[i].Equals("--oneway"))
            {
                option = "Oneway";
            }
            else if (args[i].Equals("--batch"))
            {
                batch = true;
            }
            else if (args[i].Equals("--id"))
            {
                ++i;
                if (i >= args.Length)
                {
                    Usage();
                    return 1;
                }
                id = args[i];
            }
            else if (args[i].Equals("--retryCount"))
            {
                ++i;
                if (i >= args.Length)
                {
                    Usage();
                    return 1;
                }
                retryCount = args[i];
            }
            else if (args[i].StartsWith("--"))
            {
                Usage();
                return 1;
            }
            else
            {
                topicName = args[i++];
                break;
            }

            if (!oldoption.Equals(option) && !oldoption.Equals("None"))
            {
                Usage();
                return 1;
            }
        }

        if (i != args.Length)
        {
            Usage();
            return 1;
        }

        if (batch && (option.Equals("Twoway") || option.Equals("Ordered")))
        {
            Console.WriteLine("batch can only be set with oneway or datagram");
            return 1;
        }

        IceStorm.TopicManagerPrx manager = IceStorm.TopicManagerPrxHelper.checkedCast(
            communicator.propertyToProxy("TopicManager.Proxy"));
        if (manager == null)
        {
            Console.WriteLine("invalid proxy");
            return 1;
        }

        // Retrieve the topic.
        IceStorm.TopicPrx topic;
        try
        {
            topic = manager.retrieve(topicName);
        }
        catch (IceStorm.NoSuchTopic)
        {
            try
            {
                topic = manager.create(topicName);
            }
            catch (IceStorm.TopicExists)
            {
                Console.WriteLine("temporary error. try again.");
                return 1;
            }
        }

        Ice.ObjectAdapter adapter = communicator.createObjectAdapter("Clock.Subscriber");
        // Add a servant for the Ice object. If --id is used the
        // identity comes from the command line, otherwise a UUID is
        // used.
        //
        // id is not directly altered since it is used below to
        // detect whether subscribeAndGetPublisher can raise
        // AlreadySubscribed.
        Ice.Identity subId = new Ice.Identity(id, "");
        if (subId.name == null)
        {
            subId.name = Guid.NewGuid().ToString();
        }
        Ice.ObjectPrx subscriber = adapter.add(new ClockI(), subId);

        // Activate the object adapter before subscribing.
        adapter.activate();

        Dictionary<string, string> qos = new Dictionary<string, string>();
        if (retryCount != null)
        {
            qos["retryCount"] = retryCount;
        }
        // Set up the proxy.
        if (option.Equals("Datagram"))
        {
            if (batch)
            {
                subscriber = subscriber.ice_batchDatagram();
            }
            else
            {
                subscriber = subscriber.ice_datagram();
            }
        }
        else if (option.Equals("Twoway"))
        {
            // Do nothing to the subscriber proxy. Its already twoway.
        }
        else if (option.Equals("Ordered"))
        {
            // Do nothing to the subscriber proxy. Its already twoway.
            qos["reliability"] = "ordered";
        }
        else if (option.Equals("Oneway") || option.Equals("None"))
        {
            if (batch)
            {
                subscriber = subscriber.ice_batchOneway();
            }
            else
            {
                subscriber = subscriber.ice_oneway();
            }
        }

        try
        {
            topic.subscribeAndGetPublisher(qos, subscriber);
        }
        catch (IceStorm.AlreadySubscribed)
        {
            // This should never occur when subscribing with an UUID
            Debug.Assert(id != null);
            Console.WriteLine("reactivating persistent subscriber");
        }

        communicator.waitForShutdown();
        topic.unsubscribe(subscriber);

        return 0;
    }

    private static void Usage()
    {
        Console.WriteLine("Usage: [--batch] [--datagram|--twoway|--ordered|--oneway] " +
                          "[--retryCount count] [--id id] [topic]");
    }
}
