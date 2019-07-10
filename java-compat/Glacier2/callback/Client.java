//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Demo.*;

public class Client
{
    public static void main(String[] args)
    {
        int status = 0;
        Ice.StringSeqHolder argsHolder = new Ice.StringSeqHolder(args);

        //
        // Try with resources block - communicator is automatically destroyed
        // at the end of this try block
        //
        try(Ice.Communicator communicator = Ice.Util.initialize(argsHolder, "config.client"))
        {
            if(argsHolder.value.length > 0)
            {
                System.err.println("too many arguments");
                status = 1;
            }
            else
            {
                status = run(communicator);
            }
        }

        System.exit(status);
    }

    private static int run(Ice.Communicator communicator)
    {
        Glacier2.RouterPrx router = Glacier2.RouterPrxHelper.checkedCast(communicator.getDefaultRouter());
        Glacier2.SessionPrx session;

        //
        // Loop until we have successfully create a session.
        //
        while(true)
        {
            //
            // Prompt the user for the credentials to create the session.
            //
            java.io.BufferedReader in = new java.io.BufferedReader(new java.io.InputStreamReader(System.in));
            System.out.println("This demo accepts any user-id / password combination.");

            String id;
            String pw;
            try
            {
                System.out.print("user id: ");
                System.out.flush();
                id = in.readLine();

                System.out.print("password: ");
                System.out.flush();
                pw = in.readLine();
            }
            catch(java.io.IOException ex)
            {
                ex.printStackTrace();
                continue;
            }

            //
            // Try to create a session and break the loop if succeed,
            // otherwise try again after printing the error message.
            //
            try
            {
                session = router.createSession(id, pw);
                break;
            }
            catch(Glacier2.PermissionDeniedException ex)
            {
                System.out.println("permission denied:\n" + ex.reason);
            }
            catch(Glacier2.CannotCreateSessionException ex)
            {
                System.out.println("cannot create session:\n" + ex.reason);
            }
        }

        int acmTimeout = router.getACMTimeout();
        Ice.Connection connection = router.ice_getCachedConnection();
        assert(connection != null);
        connection.setACM(new Ice.IntOptional(acmTimeout),
                          null,
                          new Ice.Optional<Ice.ACMHeartbeat>(Ice.ACMHeartbeat.HeartbeatAlways));
        connection.setCloseCallback(new CloseCallbackI());

        //
        // The Glacier2 router routes bidirectional calls to objects in the client only
        // when these objects have the correct Glacier2-issued category. The purpose of
        // the callbackReceiverFakeIdent is to demonstrate this.
        //
        // The Identity name is not checked by the server any value can be used.
        //
        Ice.Identity callbackReceiverIdent =
            new Ice.Identity(java.util.UUID.randomUUID().toString(), router.getCategoryForClient());
        Ice.Identity callbackReceiverFakeIdent =
            new Ice.Identity(java.util.UUID.randomUUID().toString(), "fake");

        Ice.ObjectPrx base = communicator.propertyToProxy("Callback.Proxy");
        CallbackPrx twoway = CallbackPrxHelper.checkedCast(base);
        CallbackPrx oneway = CallbackPrxHelper.uncheckedCast(twoway.ice_oneway());
        CallbackPrx batchOneway = CallbackPrxHelper.uncheckedCast(twoway.ice_batchOneway());

        Ice.ObjectAdapter adapter = communicator.createObjectAdapterWithRouter("", router);
        //
        // Callback will never be called for a fake identity.
        //
        adapter.add(new CallbackReceiverI(), callbackReceiverFakeIdent);

        CallbackReceiverPrx twowayR = CallbackReceiverPrxHelper.uncheckedCast(adapter.add(new CallbackReceiverI(),
                                                                                          callbackReceiverIdent));
        adapter.activate();

        CallbackReceiverPrx onewayR = CallbackReceiverPrxHelper.uncheckedCast(twowayR.ice_oneway());

        menu();

        //
        // Client REPL
        //
        String line = null;
        String override = null;
        boolean fake = false;
        java.io.BufferedReader in = new java.io.BufferedReader(new java.io.InputStreamReader(System.in));
        do
        {
            System.out.print("==> ");
            System.out.flush();
            try
            {
                line = in.readLine();
            }
            catch(java.io.IOException ex)
            {
                ex.printStackTrace();
                line = null;
            }

            if(line == null)
            {
                break;
            }

            if(line.equals("t"))
            {
                twoway.initiateCallback(twowayR);
            }
            else if(line.equals("o"))
            {
                java.util.Map<String, String> context = new java.util.HashMap<String, String>();
                if(override != null)
                {
                    context.put("_ovrd", override);
                }
                oneway.initiateCallback(onewayR, context);
            }
            else if(line.equals("O"))
            {
                java.util.Map<String, String> context = new java.util.HashMap<String, String>();
                context.put("_fwd", "O");
                if(override != null)
                {
                    context.put("_ovrd", override);
                }
                batchOneway.initiateCallback(onewayR, context);
            }
            else if(line.equals("f"))
            {
                batchOneway.ice_flushBatchRequests();
            }
            else if(line.equals("v"))
            {
                if(override == null)
                {
                    override = "some_value";
                    System.out.println("override context field is now `" + override + "'");
                }
                else
                {
                    override = null;
                    System.out.println("override context field is empty");
                }
            }
            else if(line.equals("F"))
            {
                fake = !fake;

                if(fake)
                {
                    twowayR = CallbackReceiverPrxHelper.uncheckedCast(twowayR.ice_identity(callbackReceiverFakeIdent));
                    onewayR = CallbackReceiverPrxHelper.uncheckedCast(onewayR.ice_identity(callbackReceiverFakeIdent));
                }
                else
                {
                    twowayR = CallbackReceiverPrxHelper.uncheckedCast(twowayR.ice_identity(callbackReceiverIdent));
                    onewayR = CallbackReceiverPrxHelper.uncheckedCast(onewayR.ice_identity(callbackReceiverIdent));
                }

                System.out.println("callback receiver identity: " +
                                   Ice.Util.identityToString(twowayR.ice_getIdentity()));
            }
            else if(line.equals("s"))
            {
                twoway.shutdown();
            }
            else if(line.equals("x"))
            {
                // Nothing to do
            }
            else if(line.equals("?"))
            {
                menu();
            }
            else
            {
                System.out.println("unknown command `" + line + "'");
                menu();
            }
        }
        while(!line.equals("x"));
        return 0;
    }

    private static class CloseCallbackI implements Ice.CloseCallback
    {
        @Override
        public void closed(Ice.Connection con)
        {
            System.out.println("The Glacier2 session has been destroyed.");
        }
    }

    private static void
    menu()
    {
        System.out.println(
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
