// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Diagnostics;
using ZeroC.Ice;
using ZeroC.IceStorm;

var properties = new Dictionary<string, string>();
properties.ParseArgs(ref args, "Clock");
// using statement - communicator is automatically destroyed at the end of this statement
using var communicator = new Communicator(ref args, ConfigurationManager.AppSettings, properties: properties);

// Shutdown the communicator on Ctrl+C or Ctrl+Break (always use Cancel = true with shutdown)
Console.CancelKeyPress += (sender, eventArgs) =>
{
    eventArgs.Cancel = true;
    communicator.DisposeAsync();
};

string topicName = "time";
bool ordered = false;
string? id = null;
string? retryCount = null;
int i;
for (i = 0; i < args.Length; ++i)
{
    if (args[i] == "--ordered")
    {
        ordered = true;
    }
    else if (args[i] == "--id")
    {
        ++i;
        if (i >= args.Length)
        {
            Usage();
            return;
        }
        id = args[i];
    }
    else if (args[i] == "--retryCount")
    {
        ++i;
        if (i >= args.Length)
        {
            Usage();
            return;
        }
        retryCount = args[i];
    }
    else if (args[i] == "--")
    {
        Usage();
        throw new ArgumentException($"invalid argument `{args[i]}'");
    }
    else
    {
        topicName = args[i++];
        break;
    }
}

if (i != args.Length)
{
    Usage();
    return;
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

ObjectAdapter adapter = communicator.CreateObjectAdapter("Clock.Subscriber");

// Add a servant for the Ice object. If --id is used the identity comes from the command line, otherwise a UUID is
// used. The id is not directly altered since it is used below to detect whether subscribeAndGetPublisher can raise
// AlreadySubscribed.
var subId = new Identity(id ?? Guid.NewGuid().ToString(), "");
IClockPrx subscriber = adapter.Add(subId, new Clock(), IClockPrx.Factory);

// Activate the object adapter before subscribing.
adapter.Activate();

var qos = new Dictionary<string, string>();
if (retryCount != null)
{
    qos["retryCount"] = retryCount;
}

// Set up the proxy.
if (ordered)
{
    // Do nothing to the subscriber proxy. Its already twoway.
    qos["reliability"] = "ordered";
}

try
{
    topic.SubscribeAndGetPublisher(qos, subscriber);
}
catch (AlreadySubscribed)
{
    // This should never occur when subscribing with an UUID
    Debug.Assert(id != null);
    Console.WriteLine("reactivating persistent subscriber");
}
communicator.WaitForShutdown();

static void Usage()
{
    Console.WriteLine("Usage: --ordered [--retryCount count] [--id id] [topic]");
}

namespace Demo
{
    public class Clock : IClock
    {
        public void Tick(string date, Current current) => Console.WriteLine(date);
    }
}
