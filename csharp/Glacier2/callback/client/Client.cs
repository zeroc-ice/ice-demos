// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Diagnostics;
using ZeroC.Glacier2;
using ZeroC.Ice;

// The new communicator is automatically destroyed (disposed) at the end of the using statement
using var communicator = new Communicator(ref args, ConfigurationManager.AppSettings);

// The communicator initialization removes all Ice-related arguments from args

if (args.Length > 0)
{
    throw new ArgumentException("too many arguments");
}

ZeroC.Glacier2.IRouterPrx router = communicator.DefaultRouter?.Clone(factory: ZeroC.Glacier2.IRouterPrx.Factory) ??
    throw new ArgumentException("Ice.Default.Router not set");

ISessionPrx? session;

// Loop until we have successfully create a session.
while (true)
{
    //
    // Prompt the user for the credentials to create the session.
    //
    Console.WriteLine("This demo accepts any user-id / password combination.");

    string? id;
    string? pw;
    try
    {
        Console.Write("user id: ");
        Console.Out.Flush();
        id = Console.In.ReadLine();
        if (id == null)
        {
            return;
        }
        id = id.Trim();

        Console.Write("password: ");
        Console.Out.Flush();
        pw = Console.In.ReadLine();
        if (pw == null)
        {
            return;
        }
        pw = pw.Trim();
    }
    catch (System.IO.IOException ex)
    {
        Console.WriteLine(ex);
        continue;
    }

    // Try to create a session and break the loop if succeed, otherwise try again after printing the error message.
    try
    {
        session = router.CreateSession(id, pw);
        break;
    }
    catch (PermissionDeniedException ex)
    {
        Console.WriteLine($"permission denied:\n{ex.Reason}");
    }
    catch (CannotCreateSessionException ex)
    {
        Console.WriteLine($"cannot create session:\n{ex.Reason}");
    }
}

int acmTimeout = router.GetACMTimeout();
Connection? connection = router.GetCachedConnection();
Debug.Assert(connection != null);
connection.Acm = new Acm(TimeSpan.FromSeconds(acmTimeout), AcmClose.Off, AcmHeartbeat.Always);
connection.Closed += (sender, args) => Console.WriteLine("The Glacier2 session has been destroyed.");

// The Glacier2 router routes bidirectional calls to objects in the client only
// when these objects have the correct Glacier2-issued category. The purpose of
// the callbackReceiverFakeIdent is to demonstrate this.
//
// The Identity name is not checked by the server any value can be used.
var callbackReceiverIdent = new Identity(Guid.NewGuid().ToString(), router.GetCategoryForClient());
var callbackReceiverFakeIdent = new Identity(Guid.NewGuid().ToString(), "fake");

ICallbackPrx twoway = communicator.GetPropertyAsProxy("Callback.Proxy", ICallbackPrx.Factory) ??
    throw new ArgumentException("invalid proxy");

ObjectAdapter adapter = communicator.CreateObjectAdapterWithRouter(router);

// Callback will never be called for a fake identity.
ICallbackReceiverPrx? twowayR = adapter.Add(callbackReceiverIdent,
                                           new CallbackReceiver(),
                                           ICallbackReceiverPrx.Factory);
adapter.Activate();

Menu();

// Client REPL
string? line = null;
var context = new Dictionary<string, string>();
bool fake = false;
do
{
    Console.Write("==> ");
    Console.Out.Flush();
    line = Console.In.ReadLine();
    if (line == null)
    {
        break;
    }
    if (line == "t")
    {
        twoway.InitiateCallback(twowayR, context: context);
    }
    else if (line == "v")
    {
        if (context.Count == 0)
        {
            context["_ovrd"] = "some_value";
            Console.WriteLine($"override context field is now `some_value'");
        }
        else
        {
            context.Clear();
            Console.WriteLine("override context field is empty");
        }
    }
    else if (line == "F")
    {
        fake = !fake;

        if (fake)
        {
            twowayR = twowayR.Clone(ICallbackReceiverPrx.Factory, identity: callbackReceiverFakeIdent);
        }
        else
        {
            twowayR = twowayR.Clone(ICallbackReceiverPrx.Factory, identity: callbackReceiverIdent);
        }

        Console.WriteLine($"callback receiver identity: {twowayR.Identity}");
    }
    else if (line == "s")
    {
        twoway.Shutdown();
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
        Console.WriteLine($"unknown command `{line}'");
        Menu();
    }
}
while (line != "x");

static void Menu()
{
    Console.WriteLine(
@"usage:
    t: send callback as twoway
    v: set/reset override context field
    F: set/reset fake category
    s: shutdown server
    x: exit
    ?: help
");
}
