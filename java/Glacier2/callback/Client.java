// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import com.zeroc.demos.Glacier2.callback.Demo.*;

public class Client
{
    public static void main(String[] args)
    {
        int status = 0;
        java.util.List<String> extraArgs = new java.util.ArrayList<>();

        //
        // Try with resources block - communicator is automatically destroyed
        // at the end of this try block
        //
        try(com.zeroc.Ice.Communicator communicator = com.zeroc.Ice.Util.initialize(args, "config.client", extraArgs))
        {
            if(!extraArgs.isEmpty())
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

    private static int run(com.zeroc.Ice.Communicator communicator)
    {
        com.zeroc.Glacier2.RouterPrx router = com.zeroc.Glacier2.RouterPrx.checkedCast(communicator.getDefaultRouter());
        com.zeroc.Glacier2.SessionPrx session;

        while(true)
        {
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

            try
            {
                session = router.createSession(id, pw);
                break;
            }
            catch(com.zeroc.Glacier2.PermissionDeniedException ex)
            {
                System.out.println("permission denied:\n" + ex.reason);
            }
            catch(com.zeroc.Glacier2.CannotCreateSessionException ex)
            {
                System.out.println("cannot create session:\n" + ex.reason);
            }
        }

        int acmTimeout = router.getACMTimeout();
        com.zeroc.Ice.Connection connection = router.ice_getCachedConnection();
        assert(connection != null);
        connection.setACM(java.util.OptionalInt.of(acmTimeout), null,
                          java.util.Optional.of(com.zeroc.Ice.ACMHeartbeat.HeartbeatAlways));
        connection.setCloseCallback(con -> System.out.println("The Glacier2 session has been destroyed."));

        com.zeroc.Ice.Identity callbackReceiverIdent =
            new com.zeroc.Ice.Identity("callbackReceiver", router.getCategoryForClient());

        com.zeroc.Ice.Identity callbackReceiverFakeIdent = new com.zeroc.Ice.Identity("fake", "callbackReceiver");

        com.zeroc.Ice.ObjectPrx base = communicator.propertyToProxy("Callback.Proxy");
        CallbackPrx twoway = CallbackPrx.checkedCast(base);
        CallbackPrx oneway = twoway.ice_oneway();
        CallbackPrx batchOneway = twoway.ice_batchOneway();

        com.zeroc.Ice.ObjectAdapter adapter = communicator.createObjectAdapterWithRouter("", router);
        adapter.add(new CallbackReceiverI(), callbackReceiverFakeIdent);

        CallbackReceiverPrx twowayR = CallbackReceiverPrx.uncheckedCast(
                                                                        adapter.add(new CallbackReceiverI(), callbackReceiverIdent));
        adapter.activate();
        CallbackReceiverPrx onewayR = twowayR.ice_oneway();

        menu();

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
                java.util.Map<String, String> context = new java.util.HashMap<>();
                context.put("_fwd", "t");
                if(override != null)
                {
                    context.put("_ovrd", override);
                }
                twoway.initiateCallback(twowayR, context);
            }
            else if(line.equals("o"))
            {
                java.util.Map<String, String> context = new java.util.HashMap<>();
                context.put("_fwd", "o");
                if(override != null)
                {
                    context.put("_ovrd", override);
                }
                oneway.initiateCallback(onewayR, context);
            }
            else if(line.equals("O"))
            {
                java.util.Map<String, String> context = new java.util.HashMap<>();
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
                    twowayR = CallbackReceiverPrx.uncheckedCast(twowayR.ice_identity(callbackReceiverFakeIdent));
                    onewayR = CallbackReceiverPrx.uncheckedCast(onewayR.ice_identity(callbackReceiverFakeIdent));
                }
                else
                {
                    twowayR = CallbackReceiverPrx.uncheckedCast(twowayR.ice_identity(callbackReceiverIdent));
                    onewayR = CallbackReceiverPrx.uncheckedCast(onewayR.ice_identity(callbackReceiverIdent));
                }

                System.out.println("callback receiver identity: " +
                                   com.zeroc.Ice.Util.identityToString(twowayR.ice_getIdentity()));
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

    private static void menu()
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
            "r: restart the session\n" +
            "x: exit\n" +
            "?: help\n");
    }
}
