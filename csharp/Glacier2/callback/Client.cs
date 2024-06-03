// Copyright (c) ZeroC, Inc.

using Demo;
using System.Diagnostics;

public class Client
{
    public static int Main(string[] args)
    {
        int status = 0;

        try
        {
            // The new communicator is automatically destroyed (disposed) at the end of the
            // using statement
            using var communicator = Ice.Util.initialize(ref args, "config.client");
            // The communicator initialization removes all Ice-related arguments from args
            if (args.Length > 0)
            {
                Console.Error.WriteLine("too many arguments");
                status = 1;
            }
            else
            {
                status = Run(communicator);
            }
        }
        catch (Exception ex)
        {
            Console.Error.WriteLine(ex);
            status = 1;
        }

        return status;
    }

    private static int Run(Ice.Communicator communicator)
    {
        var router = Glacier2.RouterPrxHelper.checkedCast(communicator.getDefaultRouter());
        Glacier2.SessionPrx session;
        // Loop until we have successfully create a session.
        while (true)
        {
            // Prompt the user for the credentials to create the session.
            Console.WriteLine("This demo accepts any user-id / password combination.");

            string id;
            string pw;
            try
            {
                Console.Write("user id: ");
                Console.Out.Flush();
                id = Console.In.ReadLine();
                if (id == null)
                {
                    throw new Ice.CommunicatorDestroyedException();
                }
                id = id.Trim();

                Console.Write("password: ");
                Console.Out.Flush();
                pw = Console.In.ReadLine();
                if (pw == null)
                {
                    throw new Ice.CommunicatorDestroyedException();
                }
                pw = pw.Trim();
            }
            catch (System.IO.IOException ex)
            {
                Console.WriteLine(ex.StackTrace.ToString());
                continue;
            }

            // Try to create a session and break the loop if succeed,
            // otherwise try again after printing the error message.
            try
            {
                session = router.createSession(id, pw);
                break;
            }
            catch (Glacier2.PermissionDeniedException ex)
            {
                Console.WriteLine("permission denied:\n" + ex.reason);
            }
            catch (Glacier2.CannotCreateSessionException ex)
            {
                Console.WriteLine("cannot create session:\n" + ex.reason);
            }
        }

        int acmTimeout = router.getACMTimeout();
        Ice.Connection connection = router.ice_getCachedConnection();
        Debug.Assert(connection != null);
        connection.setCloseCallback(_ => Console.WriteLine("The Glacier2 session has been destroyed."));

        // The Glacier2 router routes bidirectional calls to objects in the client only
        // when these objects have the correct Glacier2-issued category. The purpose of
        // the callbackReceiverFakeIdent is to demonstrate this.
        //
        // The Identity name is not checked by the server any value can be used.
        Ice.Identity callbackReceiverIdent =
            new Ice.Identity(System.Guid.NewGuid().ToString(), router.getCategoryForClient());
        Ice.Identity callbackReceiverFakeIdent =
            new Ice.Identity(System.Guid.NewGuid().ToString(), "fake");

        Ice.ObjectPrx @base = communicator.propertyToProxy("Callback.Proxy");
        CallbackPrx twoway = CallbackPrxHelper.checkedCast(@base);
        CallbackPrx oneway = CallbackPrxHelper.uncheckedCast(twoway.ice_oneway());
        CallbackPrx batchOneway = CallbackPrxHelper.uncheckedCast(twoway.ice_batchOneway());

        var adapter = communicator.createObjectAdapterWithRouter("", router);
        // Callback will never be called for a fake identity.
        adapter.add(new CallbackReceiverI(), callbackReceiverFakeIdent);

        CallbackReceiverPrx twowayR = CallbackReceiverPrxHelper.uncheckedCast(
            adapter.add(new CallbackReceiverI(), callbackReceiverIdent));

        adapter.activate();
        CallbackReceiverPrx onewayR = CallbackReceiverPrxHelper.uncheckedCast(twowayR.ice_oneway());

        Menu();

        // Client REPL
        string line = null;
        string @override = null;
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
            if (line.Equals("t"))
            {
                twoway.initiateCallback(twowayR);
            }
            else if (line.Equals("o"))
            {
                Dictionary<string, string> context = new Dictionary<string, string>();
                if (@override != null)
                {
                    context["_ovrd"] = @override;
                }
                oneway.initiateCallback(onewayR, context);
            }
            else if (line.Equals("O"))
            {
                Dictionary<string, string> context = new Dictionary<string, string>();
                context["_fwd"] = "O";
                if (@override != null)
                {
                    context["_ovrd"] = @override;
                }
                batchOneway.initiateCallback(onewayR, context);
            }
            else if (line.Equals("f"))
            {
                batchOneway.ice_flushBatchRequests();
            }
            else if (line.Equals("v"))
            {
                if (@override == null)
                {
                    @override = "some_value";
                    Console.WriteLine("override context field is now `" + @override + "'");
                }
                else
                {
                    @override = null;
                    Console.WriteLine("override context field is empty");
                }
            }
            else if (line.Equals("F"))
            {
                fake = !fake;

                if (fake)
                {
                    twowayR = CallbackReceiverPrxHelper.uncheckedCast(
                        twowayR.ice_identity(callbackReceiverFakeIdent));
                    onewayR = CallbackReceiverPrxHelper.uncheckedCast(
                        onewayR.ice_identity(callbackReceiverFakeIdent));
                }
                else
                {
                    twowayR = CallbackReceiverPrxHelper.uncheckedCast(
                        twowayR.ice_identity(callbackReceiverIdent));
                    onewayR = CallbackReceiverPrxHelper.uncheckedCast(
                        onewayR.ice_identity(callbackReceiverIdent));
                }

                Console.WriteLine("callback receiver identity: " +
                                    Ice.Util.identityToString(twowayR.ice_getIdentity()));
            }
            else if (line.Equals("s"))
            {
                twoway.shutdown();
            }
            else if (line.Equals("x"))
            {
                // Nothing to do
            }
            else if (line.Equals("?"))
            {
                Menu();
            }
            else
            {
                Console.WriteLine("unknown command `" + line + "'");
                Menu();
            }
        }
        while (!line.Equals("x"));

        return 0;
    }

    private static void Menu()
    {
        Console.WriteLine(
            "usage:\n" +
            "t: send callback as twoway\n" +
            "o: send callback as oneway\n" +
            "O: send callback as batch oneway\n" +
            "f: flush all batch requests\n" +
            "v: set/reset override context field\n" +
            "F: set/reset fake category\n" +
            "s: shutdown server\n" +
            "x: exit\n" +
            "?: help\n");
    }
}
