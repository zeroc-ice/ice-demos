// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using System;
using System.Configuration;
using ZeroC.Ice;

// The new communicator is automatically destroyed (disposed) at the end of the using statement
using var communicator = new Communicator(ref args, ConfigurationManager.AppSettings);

// The communicator initialization removes all Ice-related arguments from args
if (args.Length > 0)
{
    throw new ArgumentException("too many arguments");
}

ICallbackSenderPrx sender = communicator.GetPropertyAsProxy("CallbackSender.Proxy", ICallbackSenderPrx.Factory) ??
    throw new ArgumentException("invalid proxy");

ObjectAdapter adapter = communicator.CreateObjectAdapter("Callback.Client");
ICallbackReceiverPrx receiver = adapter.Add("callbackReceiver", new CallbackReceiver(), ICallbackReceiverPrx.Factory);
adapter.Activate();

Menu();

string? line = null;
do
{
    try
    {
        Console.Out.Write("==> ");
        Console.Out.Flush();
        line = Console.In.ReadLine();
        if (line == null)
        {
            break;
        }
        if (line == "t")
        {
            sender.InitiateCallback(receiver);
        }
        else if (line == "s")
        {
            sender.Shutdown();
        }
        else if (line == "x")
        {
            // Nothing to do
        }
        else if (line == "?")
        {
            Menu();
        }
        else
        {
            Console.Out.WriteLine($"unknown command `{line}'");
            Menu();
        }
    }
    catch (Exception ex)
    {
        Console.Error.WriteLine(ex);
    }
}
while (line != "x");

static void Menu()
{
    Console.Out.Write(
@"usage:
    t: send callback
    s: shutdown server
    x: exit
    ?: help
");
}
