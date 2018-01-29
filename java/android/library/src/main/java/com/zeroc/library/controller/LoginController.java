// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

package com.zeroc.library.controller;

import android.os.Build;
import android.content.res.Resources;
import android.os.Handler;

import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;

import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.Connection;
import com.zeroc.Ice.InitializationData;
import com.zeroc.Ice.LocalException;
import com.zeroc.Ice.ObjectPrx;
import com.zeroc.Ice.Util;
import com.zeroc.Glacier2.CannotCreateSessionException;
import com.zeroc.Glacier2.PermissionDeniedException;
import com.zeroc.Glacier2.SessionPrx;
import com.zeroc.Glacier2.SessionNotExistException;
import com.zeroc.library.R;

public class LoginController
{
    public interface Listener
    {
        void onLoginInProgress();
        void onLogin(SessionController controller);
        void onLoginError();
    }

    private boolean _destroyed = false;
    private Handler _handler;
    private Communicator _communicator;
    private String _loginError;
    private Listener _loginListener;
    private SessionController _sessionController;

    synchronized private void postLoginFailure(final String loginError)
    {
        _loginError = loginError;
        if(_loginListener != null)
        {
            final Listener listener = _loginListener;
            _handler.post(new Runnable()
            {
                public void run()
                {
                    listener.onLoginError();
                }
            });
        }
    }

    public LoginController(final Resources resources, final String username, final String password, Listener listener)
    {
        _handler = new Handler();
        _loginListener = listener;
        _loginListener.onLoginInProgress();

        new Thread(new Runnable()
        {
            public void run()
            {
                try
                {
                    long refreshTimeout;

                    InitializationData initData = new InitializationData();

                    initData.dispatcher = (Runnable runnable, Connection connection) ->
                    {
                        _handler.post(runnable);
                    };

                    initData.properties = Util.createProperties();

                    InputStream inputStream = resources.getAssets().open("config.properties");
                    Properties properties = new Properties();
                    properties.load(inputStream);
                    for(String name: properties.stringPropertyNames())
                    {
                        String value = properties.getProperty(name);
                        initData.properties.setProperty(name, value);
                    }

                    //
                    // Set common properties
                    //
                    initData.properties.setProperty("Ice.Trace.Network", "0");
                    initData.properties.setProperty("IceSSL.Trace.Security", "0");
                    initData.properties.setProperty("Ice.Plugin.IceSSL", "com.zeroc.IceSSL.PluginFactory");

                    //
                    // Check for Ice.Default.Router. If we find it use it, otherwise use a direct connection
                    //
                    if(!initData.properties.getProperty("Ice.Default.Router").isEmpty())
                    {
                        initData.properties.setProperty("Ice.RetryIntervals", "-1");
                    }

                    if(initData.properties.getPropertyAsIntWithDefault("IceSSL.UsePlatformCAs", 0) == 0)
                    {
                        //
                        // We need to postpone plug-in initialization so that we can configure IceSSL
                        // with a resource stream for the certificate information.
                        //
                        initData.properties.setProperty("Ice.InitPlugins", "0");
                        _communicator = Util.initialize(initData);

                        com.zeroc.IceSSL.Plugin plugin =
                            (com.zeroc.IceSSL.Plugin)_communicator.getPluginManager().getPlugin("IceSSL");
                        java.io.InputStream certStream = resources.openRawResource(R.raw.client);
                        plugin.setTruststoreStream(certStream);

                        _communicator.getPluginManager().initializePlugins();
                    }
                    else
                    {
                        _communicator = Util.initialize(initData);
                    }

                    SessionAdapter session = null;

                    com.zeroc.Ice.Connection con = null;
                    if(_communicator.getDefaultRouter() != null)
                    {
                        final com.zeroc.Ice.RouterPrx r = _communicator.getDefaultRouter();
                        final com.zeroc.Glacier2.RouterPrx router = com.zeroc.Glacier2.RouterPrx.checkedCast(r);

                        if(router == null)
                        {
                            postLoginFailure("Glacier2 proxy is invalid");
                            return;
                        }

                        SessionPrx glacier2session = router.createSession(username, password);

                        final Demo.Glacier2SessionPrx sess = Demo.Glacier2SessionPrx.uncheckedCast(glacier2session);
                        final Demo.LibraryPrx library = sess.getLibrary();
                        refreshTimeout = router.getSessionTimeout();
                        con = sess.ice_getConnection();

                        session = new SessionAdapter()
                        {
                            public void destroy()
                            {
                                try
                                {
                                    router.destroySession();
                                }
                                catch(SessionNotExistException e)
                                {
                                }
                            }

                            public Demo.LibraryPrx getLibrary()
                            {
                                return library;
                            }
                        };
                    }
                    else
                    {
                        ObjectPrx proxy = _communicator.stringToProxy("LibraryDemo.Proxy");

                        Demo.SessionFactoryPrx factory = Demo.SessionFactoryPrx.checkedCast(proxy);
                        if(factory == null)
                        {
                            postLoginFailure("SessionFactory proxy is invalid.");
                            return;
                        }

                        final Demo.SessionPrx sess = factory.create();
                        final Demo.LibraryPrx library = sess.getLibrary();
                        refreshTimeout = factory.getSessionTimeout();
                        con = sess.ice_getConnection();

                        session = new SessionAdapter()
                        {
                            public void destroy()
                            {
                                sess.destroy();
                            }

                            public Demo.LibraryPrx getLibrary()
                            {
                                return library;
                            }
                        };
                    }

                    //
                    // Configure the connection to send heartbeats in order to keep our session alive.
                    //
                    con.setACM(java.util.OptionalInt.of((int)refreshTimeout),
                               java.util.Optional.of(com.zeroc.Ice.ACMClose.CloseOff),
                               java.util.Optional.of(com.zeroc.Ice.ACMHeartbeat.HeartbeatAlways));

                    synchronized(LoginController.this)
                    {
                        if(_destroyed)
                        {
                            // Here the app was terminated while session establishment was in progress.
                            try
                            {
                                session.destroy();
                            }
                            catch(Exception e)
                            {
                            }

                            _communicator.destroy();
                            return;
                        }

                        _sessionController = new SessionController(_handler, _communicator, session, username);
                        if(_loginListener != null)
                        {
                            final Listener listener = _loginListener;
                            _handler.post(new Runnable()
                            {
                                public void run()
                                {
                                    listener.onLogin(_sessionController);
                                }
                            });
                        }
                    }
                }
                catch(CannotCreateSessionException ex)
                {
                    ex.printStackTrace();
                    postLoginFailure(String.format("Session creation failed: %s", ex.reason));
                }
                catch(PermissionDeniedException ex)
                {
                    ex.printStackTrace();
                    postLoginFailure(String.format("Login failed: %s", ex.reason));
                }
                catch(LocalException ex)
                {
                    ex.printStackTrace();
                    postLoginFailure(String.format("Login failed: %s", ex.toString()));
                }
                catch(IOException ex)
                {
                    ex.printStackTrace();
                    postLoginFailure(String.format("Loading config failed: %s", ex.toString()));
                }
            }
        }).start();
    }

    synchronized public void destroy()
    {
        if(_destroyed)
        {
            return;
        }
        _destroyed = true;
        //
        // There are three cases:
        //
        // 1. A session has been created. In this case the communicator is owned
        // by the session controller.
        //
        // 2. The session creation failed. In this case _loginError is non-null.
        // Destroy the communicator.
        //
        // 3. The session creation is in progress. In which case, things will be
        // cleaned up once the session establishment is complete.
        //
        if(_sessionController == null && _loginError != null)
        {
            _communicator.destroy();
        }
    }

    synchronized public String getLoginError()
    {
        return _loginError;
    }

    synchronized public void setLoginListener(Listener listener)
    {
        _loginListener = listener;
        if(listener != null)
        {
            if(_loginError != null)
            {
                listener.onLoginError();
            }
            else if(_sessionController == null)
            {
                listener.onLoginInProgress();
            }
            else
            {
                listener.onLogin(_sessionController);
            }
        }
    }
}
