//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Database.library.Demo.*;

import com.zeroc.Ice.ACMClose;
import com.zeroc.Ice.ACMHeartbeat;

class RunParser
{
    //
    // Adapter for the two types of session objects.
    //
    interface SessionAdapter
    {
        public LibraryPrx getLibrary();
        public void destroy();
    }

    static SessionAdapter createSession(String appName, com.zeroc.Ice.Communicator communicator)
    {
        SessionAdapter session;
        final com.zeroc.Glacier2.RouterPrx router =
            com.zeroc.Glacier2.RouterPrx.uncheckedCast(communicator.getDefaultRouter());
        if(router != null)
        {
            com.zeroc.Glacier2.SessionPrx glacier2session = null;
            long timeout;
            java.io.BufferedReader in = new java.io.BufferedReader(new java.io.InputStreamReader(System.in));
            while(true)
            {
                System.out.println("This demo accepts any user-id / password combination.");
                try
                {
                    String id;
                    System.out.print("user id: ");
                    System.out.flush();
                    id = in.readLine();

                    String pw;
                    System.out.print("password: ");
                    System.out.flush();
                    pw = in.readLine();

                    try
                    {
                        glacier2session = router.createSession(id, pw);
                        final int acmTimeout = router.getACMTimeout();
                        if(acmTimeout > 0)
                        {
                            com.zeroc.Ice.Connection connection = router.ice_getCachedConnection();
                            assert(connection != null);
                            connection.setACM(
                                java.util.OptionalInt.of(acmTimeout), null,
                                java.util.Optional.of(ACMHeartbeat.HeartbeatAlways));
                        }
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
                catch(java.io.IOException ex)
                {
                    ex.printStackTrace();
                }
            }

            final Glacier2SessionPrx sess = Glacier2SessionPrx.uncheckedCast(glacier2session);
            session = new SessionAdapter()
            {
                @Override
                public LibraryPrx getLibrary()
                {
                    return sess.getLibrary();
                }

                @Override
                public void destroy()
                {
                    try
                    {
                        router.destroySession();
                    }
                    catch(com.zeroc.Glacier2.SessionNotExistException ex)
                    {
                    }
                    catch(com.zeroc.Ice.ConnectionLostException ex)
                    {
                        //
                        // Expected: the router closed the connection.
                        //
                    }
                }
            };
        }
        else
        {
            SessionFactoryPrx factory = SessionFactoryPrx.checkedCast(
                communicator.propertyToProxy("SessionFactory.Proxy"));
            if(factory == null)
            {
                System.err.println(appName + ": invalid object reference");
                return null;
            }

            final SessionPrx sess = factory.create();
            session = new SessionAdapter()
            {
                @Override
                public LibraryPrx getLibrary()
                {
                    return sess.getLibrary();
                }

                @Override
                public void destroy()
                {
                    sess.destroy();
                }
            };
        }
        return session;
    }

    static int runParser(String appName, String[] args, final com.zeroc.Ice.Communicator communicator)
    {
        final SessionAdapter session = createSession(appName, communicator);
        if(session == null)
        {
            return 1;
        }

        LibraryPrx library = session.getLibrary();

        Parser parser = new Parser(communicator, library);

        int rc = 0;

        if(args.length == 1)
        {
            rc = parser.parse(args[0]);
        }

        if(rc == 0)
        {
            rc = parser.parse();
        }

        session.destroy();

        return rc;
    }
}
