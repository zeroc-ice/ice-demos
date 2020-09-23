// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using System;
using System.Configuration;
using System.Diagnostics;
using System.Globalization;
using ZeroC.Ice;
using ZeroC.IceStorm;

// using statement - communicator is automatically destroyed at the end of this statement
using var communicator = new Communicator(ref args, ConfigurationManager.AppSettings);

// Destroy the communicator on Ctrl+C or Ctrl+Break
Console.CancelKeyPress += (sender, eventArgs) => communicator.DisposeAsync();

string topicName = args.Length == 0 ? "time" : args[0];
if (args.Length > 1)
{
    throw new ArgumentException("too many arguments");
}

ITopicManagerPrx manager = communicator.GetPropertyAsProxy("TopicManager.Proxy", ITopicManagerPrx.Factory) ??
    throw new ArgumentException("invalid proxy");

// Retrieve the topic.
ITopicPrx? topic;
try
{
    topic = manager.Retrieve(topicName);
}
catch (NoSuchTopic)
{
    try
    {
        topic = manager.Create(topicName);
    }
    catch (TopicExists)
    {
        Console.WriteLine("temporary error. try again.");
        return;
    }
}
Debug.Assert(topic != null);

// Get the topic's publisher object, and create a Clock proxy with the mode specified as an argument of this
// application.
IObjectPrx? publisher = topic.GetPublisher();
Debug.Assert(publisher != null);
IClockPrx clock = publisher.Clone(factory: IClockPrx.Factory);

Console.WriteLine("publishing tick events. Press ^C to terminate the application.");
try
{
    while (true)
    {
        clock.Tick(DateTime.Now.ToString("G", DateTimeFormatInfo.InvariantInfo));
        System.Threading.Thread.Sleep(1000);
    }
}
catch (ObjectDisposedException)
{
    // Ignore
}
