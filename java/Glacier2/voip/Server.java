// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import java.util.concurrent.ScheduledThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import com.zeroc.Glacier2.CannotCreateSessionException;
import com.zeroc.Glacier2.SessionControlPrx;
import com.zeroc.Glacier2.SessionPrx;
import com.zeroc.Glacier2.PermissionsVerifier;
import com.zeroc.Glacier2.SessionManager;
import com.zeroc.Ice.Current;
import com.zeroc.Ice.LocalException;
import com.zeroc.Ice.NotRegisteredException;
import com.zeroc.Ice.ObjectNotExistException;
import Voip.ControlPrx;
import Voip.Session;

public class Server extends com.zeroc.Ice.Application
{
    private final ScheduledThreadPoolExecutor _executor = new ScheduledThreadPoolExecutor(1);

    class SessionI implements Session
    {
        private ControlPrx _ctrl;
        private long _timestamp = System.currentTimeMillis();

        @Override
        public void setControl(ControlPrx ctrl, Current current)
        {
            _ctrl = ctrl;
        }

        @Override
        public void simulateCall(int delay, Current current)
        {
            _executor.schedule(() ->
            {
                if(_ctrl != null)
                {
                    System.out.println("calling incoming call");
                    _ctrl.incomingCallAsync().whenComplete((result, ex) ->
                        {
                            if(ex == null)
                            {
                                System.out.println("incoming call succeeded");
                            }
                            else
                            {
                                System.out.println("incoming call failed");
                                ex.printStackTrace();
                            }
                        });
                }
            }, delay, TimeUnit.MILLISECONDS);
        }

        @Override
        public void destroy(Current current)
        {
            try
            {
                current.adapter.remove(current.id);
            }
            catch(NotRegisteredException ex)
            {
                // Ignore.
            }
        }

        @Override
        public void refresh(Current current)
        {
            _timestamp = System.currentTimeMillis();
        }

        long getTimestamp()
        {
            return _timestamp;
        }
    }

    class PermissionsVerifierI implements PermissionsVerifier
    {
        @Override
        public PermissionsVerifier.CheckPermissionsResult checkPermissions(String userId, String password,
                                                                           Current current)
        {
            PermissionsVerifier.CheckPermissionsResult r = new PermissionsVerifier.CheckPermissionsResult();
            r.returnValue = true;
            return r;
        }
    }

    class SessionManagerI implements SessionManager
    {
        @Override
        public SessionPrx create(String userId, SessionControlPrx control, Current current)
            throws CannotCreateSessionException
        {
            // The configured timeout must be greater than 600. This is 601 * 2.
            final long sessionTimeout = 1202;
            final SessionI session = new SessionI();
            final com.zeroc.Ice.Identity ident = new com.zeroc.Ice.Identity();
            ident.name = java.util.UUID.randomUUID().toString();
            ident.category = "session";
            final SessionPrx proxy = SessionPrx.uncheckedCast(current.adapter.add(session, ident));
            _executor.scheduleWithFixedDelay(() ->
                {
                    // If the session has already been destroyed the ONE will
                    // fall out of run canceling the task.
                    if (System.currentTimeMillis() - session.getTimestamp() > (sessionTimeout * 1000L * 2))
                    {
                        proxy.destroy();
                        throw new ObjectNotExistException();
                    }
                }, sessionTimeout, sessionTimeout, TimeUnit.SECONDS);

            return proxy;
        }
    }

    Server()
    {
        // We want the executor to shutdown even if there are scheduled tasks.
        _executor.setExecuteExistingDelayedTasksAfterShutdownPolicy(false);
    }

    @Override
    public int run(String[] args)
    {
        if(args.length > 0)
        {
            System.err.println(appName() + ": too many arguments");
            return 1;
        }

        com.zeroc.Ice.ObjectAdapter adapter = communicator().createObjectAdapter("VoipServer");
        adapter.add(new PermissionsVerifierI(), com.zeroc.Ice.Util.stringToIdentity("VoipVerifier"));
        adapter.add(new SessionManagerI(), com.zeroc.Ice.Util.stringToIdentity("VoipSessionManager"));
        adapter.activate();
        communicator().waitForShutdown();
        _executor.shutdown();
        return 0;
    }

    public static void main(String[] args)
    {
        Server app = new Server();
        int status = app.main("Server", args, "config.server");
        System.exit(status);
    }
}
