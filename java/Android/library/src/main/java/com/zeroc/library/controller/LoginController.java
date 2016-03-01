// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

package com.zeroc.library.controller;

import android.os.Build;
import com.zeroc.library.R;

import android.content.res.Resources;
import android.os.Handler;

import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;

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
    private Ice.Communicator _communicator;
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

                    Ice.InitializationData initData = new Ice.InitializationData();

                    initData.dispatcher = new Ice.Dispatcher()
                    {
                        @Override
                        public void
                        dispatch(Runnable runnable, Ice.Connection connection)
                        {
                            _handler.post(runnable);
                        }
                    };

                    initData.properties = Ice.Util.createProperties();

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
                    initData.properties.setProperty("Ice.Plugin.IceSSL", "IceSSL.PluginFactory");

                    // SDK versions < 21 only support TLSv1 with SSLEngine.
                    if(Build.VERSION.SDK_INT < 21)
                    {
                        initData.properties.setProperty("IceSSL.Protocols", "tls1_0");
                    }

                    //
                    // Check for Ice.Default.Router. If we find it use it, otherwise use a direct connection
                    //

                    if(!initData.properties.getProperty("Ice.Default.Router").isEmpty())
                    {
                        initData.properties.setProperty("Ice.RetryIntervals", "-1");
                    }


                    if(initData.properties.getPropertyAsIntWithDefault("IceSSL.UsePlatformCAs", 0) == 0)
                    {
                        initData.properties.setProperty("Ice.InitPlugins", "0");
                        java.io.InputStream certStream = resources.openRawResource(R.raw.client);
                        _communicator = Ice.Util.initialize(initData);

                        IceSSL.Plugin plugin = (IceSSL.Plugin)_communicator.getPluginManager().getPlugin("IceSSL");
                        plugin.setTruststoreStream(certStream);

                        _communicator.getPluginManager().initializePlugins();
                    }
                    else
                    {
                    _communicator = Ice.Util.initialize(initData);
                    }


                    SessionAdapter session = null;

                    if(_communicator.getDefaultRouter() != null)
                    {
                        final Ice.RouterPrx r = _communicator.getDefaultRouter();
                        final Glacier2.RouterPrx router =  Glacier2.RouterPrxHelper.checkedCast(r);

                        if(router == null)
                        {
                            postLoginFailure("Glacier2 proxy is invalid");
                            return;
                        }

                        Glacier2.SessionPrx glacier2session = router.createSession(username, password);

                        final Demo.Glacier2SessionPrx sess =
                            Demo.Glacier2SessionPrxHelper.uncheckedCast(glacier2session);
                        final Demo.LibraryPrx library = sess.getLibrary();
                        refreshTimeout = (router.getSessionTimeout() * 1000) / 2;

                        session = new SessionAdapter()
                        {
                            public void destroy()
                            {
                                try
                                {
                                    router.destroySession();
                                }
                                catch(Glacier2.SessionNotExistException e)
                                {
                                }
                            }

                            public void refresh()
                            {
                                sess.refresh();
                            }

                            public Demo.LibraryPrx getLibrary()
                            {
                                return library;
                            }
                        };
                    }
                    else
                    {
                        Ice.ObjectPrx proxy = _communicator.stringToProxy("LibraryDemo.Proxy");

                        Demo.SessionFactoryPrx factory = Demo.SessionFactoryPrxHelper.checkedCast(proxy);
                        if(factory == null)
                        {
                            postLoginFailure("SessionFactory proxy is invalid.");
                            return;
                        }

                        final Demo.SessionPrx sess = factory.create();
                        final Demo.LibraryPrx library = sess.getLibrary();
                        refreshTimeout = (factory.getSessionTimeout() * 1000) / 2;

                        session = new SessionAdapter()
                        {
                            public void destroy()
                            {
                                sess.destroy();
                            }

                            public void refresh()
                            {
                                sess.refresh();
                            }

                            public Demo.LibraryPrx getLibrary()
                            {
                                return library;
                            }
                        };
                    }

                    synchronized(LoginController.this)
                    {
                        if(_destroyed)
                        {
                            // Here the app was terminated while session
                            // establishment was in progress.
                            try
                            {
                                session.destroy();
                            }
                            catch(Exception e)
                            {
                            }

                            try
                            {
                                _communicator.destroy();
                            }
                            catch(Exception e)
                            {
                            }
                            return;
                        }

                        _sessionController = new SessionController(_handler, _communicator, session, username, refreshTimeout);
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
                catch(final Glacier2.CannotCreateSessionException ex)
                {
                    ex.printStackTrace();
                    postLoginFailure(String.format("Session creation failed: %s", ex.reason));
                }
                catch(Glacier2.PermissionDeniedException ex)
                {
                    ex.printStackTrace();
                    postLoginFailure(String.format("Login failed: %s", ex.reason));
                }
                catch(Ice.LocalException ex)
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
            try
            {
                _communicator.destroy();
            }
            catch(Exception e)
            {
            }
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
