// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

package com.zeroc.chat.service;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;
import java.util.Properties;
import java.util.UUID;

import android.os.Build;

import android.content.res.Resources;
import android.os.Handler;

import com.zeroc.chat.R;

public class AppSession
{
    private ChatService _service;
    private Chat.ChatSessionPrx _chat;
    private List<ChatRoomListener> _listeners = new LinkedList<ChatRoomListener>();
    private LinkedList<ChatEventReplay> _replay = new LinkedList<ChatEventReplay>();
    private List<String> _users = new LinkedList<String>();

    static final int MAX_MESSAGES = 200;

    private boolean _destroyed = false;
    private String _hostname;
    private String _error;

    private Glacier2.SessionHelper _session;

    public AppSession(ChatService service, final Resources resources, final Handler handler, String username, String password)
            throws IOException
    {
        _service = service;

        Ice.InitializationData initData = new Ice.InitializationData();

        initData.dispatcher = new Ice.Dispatcher()
        {
            @Override
            public void
            dispatch(Runnable runnable, Ice.Connection connection)
            {
                handler.post(runnable);
            }
        };

        initData.properties = Ice.Util.createProperties();

        try
        {
            InputStream inputStream = resources.getAssets().open("config.properties");
            Properties properties = new Properties();
            properties.load(inputStream);
            for (String name : properties.stringPropertyNames()) {
                String value = properties.getProperty(name);
                initData.properties.setProperty(name, value);
            }
        }
        catch(IOException ex)
        {
            _error = String.format("Initialization failed %s", ex.toString());
            throw ex;
        }

        initData.properties.setProperty("Ice.RetryIntervals", "-1");
        initData.properties.setProperty("Ice.Trace.Network", "0");
        initData.properties.setProperty("Ice.Plugin.IceSSL", "IceSSL.PluginFactory");

        // SDK versions < 21 only support TLSv1 with SSLEngine.
        if(Build.VERSION.SDK_INT < 21)
        {
            initData.properties.setProperty("IceSSL.Protocols", "tls1_0");
        }

        if(initData.properties.getPropertyAsIntWithDefault("IceSSL.UsePlatformCAs", 0) == 0)
        {
            initData.properties.setProperty("Ice.InitPlugins", "0");
            initData.properties.setProperty("IceSSL.TruststoreType", "BKS");
            initData.properties.setProperty("IceSSL.Password", "password");
        }

        Glacier2.SessionFactoryHelper factory = new Glacier2.SessionFactoryHelper(initData, new Glacier2.SessionCallback()
            {
                public void
                connected(final Glacier2.SessionHelper session)
                    throws Glacier2.SessionNotExistException {
                    //
                    // If the session has been reassigned avoid the spurious callback.
                    //
                    if (session != _session) {
                        return;
                    }

                    Chat.ChatRoomCallbackPrx callback = Chat.ChatRoomCallbackPrxHelper.uncheckedCast(_session.addWithUUID(new ChatCallbackI()));

                    _chat = Chat.ChatSessionPrxHelper.uncheckedCast(_session.session());
                    try {
                        _chat.begin_setCallback(callback, new Chat.Callback_ChatSession_setCallback() {
                            @Override
                            public void
                            response() {
                                _service.loginComplete();
                            }

                            @Override
                            public void
                            exception(Ice.LocalException ex) {
                                AppSession.this.destroy();
                            }
                        });
                    } catch (Ice.CommunicatorDestroyedException ex) {
                        //Ignore client session was destroyed.
                    }
                }

                public void
                disconnected(Glacier2.SessionHelper session)
                {
                    if(!_destroyed) // Connection closed by user logout/exit
                    {
                        destroyWithError("<system-message> - The connection with the server was unexpectedly lost.\nTry again.");
                    }
                }

                public void
                connectFailed(Glacier2.SessionHelper session, Throwable exception)
                {
                    try
                    {
                        throw exception;
                    }
                    catch(final Glacier2.CannotCreateSessionException ex)
                    {
                        setError("Login failed (Glacier2.CannotCreateSessionException):\n" + ex.reason);
                    }
                    catch(final Glacier2.PermissionDeniedException ex)
                    {
                        setError("Login failed (Glacier2.PermissionDeniedException):\n" + ex.reason);
                    }
                    catch(Ice.Exception ex)
                    {
                        setError("Login failed (" + ex.ice_name() + ").\n" +
                                 "Please check your configuration.");
                    }
                    catch(final Throwable ex) {
                        setError("Login failed:\n" + stack2string(ex));
                    }
                    _service.loginFailed();
                }

                public void
                createdCommunicator(Glacier2.SessionHelper session)
                {
                    Ice.Communicator communicator = session.communicator();
                    if(communicator.getProperties().getPropertyAsIntWithDefault("IceSSL.UsePlatformCAs", 0) == 0)
                    {
                        java.io.InputStream certStream = resources.openRawResource(R.raw.client);
                        IceSSL.Plugin plugin = (IceSSL.Plugin)communicator.getPluginManager().getPlugin("IceSSL");
                        plugin.setTruststoreStream(certStream);
                        communicator.getPluginManager().initializePlugins();
                    }
                }
            });
        _session = factory.connect(username, password);
    }

    synchronized public void destroy()
    {
        if(_destroyed)
        {
            return;
        }
        _destroyed = true;

        _session.destroy();
        _session = null;
    }


    synchronized public String getError()
    {
        return _error;
    }

    synchronized private void setError(String error)
    {
        _error = error;
    }

    // This method is only called by the UI thread.
    synchronized public void send(String t)
    {
        if(_destroyed)
        {
            return;
        }

        _chat.begin_send(t, new Chat.Callback_ChatSession_send()
            {
                @Override
                public void exception(Ice.LocalException ex)
                {
                    destroyWithError(ex.toString());
                }

                @Override
                public void exception(Ice.UserException ex)
                {
                    destroyWithError(ex.toString());
                }

                @Override
                public void response(long ret)
                {
                }
            });
    }

    // This method is only called by the UI thread.
    public synchronized String addChatRoomListener(ChatRoomListener cb, boolean replay)
    {
        _listeners.add(cb);
        cb.init(_users);

        if(replay)
        {
            // Replay the entire state.
            for(ChatEventReplay r : _replay)
            {
                r.replay(cb);
            }
        }

        if(_error != null)
        {
            cb.error();
        }

        return _hostname;
    }

    // This method is only called by the UI thread.
    synchronized public void removeChatRoomListener(ChatRoomListener cb)
    {
        _listeners.remove(cb);
    }

    private interface ChatEventReplay
    {
        public void replay(ChatRoomListener cb);
    }

    private class ChatCallbackI extends Chat._ChatRoomCallbackDisp
    {
        public void init(String[] users, Ice.Current current)
        {
            List<ChatRoomListener> copy;
            final List<String> u = Arrays.asList(users);

            synchronized(this)
            {
                _users.clear();
                _users.addAll(u);

                // No replay event for init.
                copy = new ArrayList<ChatRoomListener>(_listeners);
            }

            for(ChatRoomListener listener : copy)
            {
                listener.init(u);
            }
        }

        public void join(final long timestamp, final String name, Ice.Current current)
        {
            List<ChatRoomListener> copy;

            synchronized(this)
            {
                _replay.add(new ChatEventReplay()
                {
                    public void replay(ChatRoomListener cb)
                    {
                        cb.join(timestamp, name);
                    }
                });
                if(_replay.size() > MAX_MESSAGES)
                {
                    _replay.removeFirst();
                }

                _users.add(name);

                copy = new ArrayList<ChatRoomListener>(_listeners);
            }

            for(ChatRoomListener listener : copy)
            {
                listener.join(timestamp, name);
            }
        }

        public void leave(final long timestamp, final String name, Ice.Current current)
        {
            List<ChatRoomListener> copy;

            synchronized(this)
            {
                _replay.add(new ChatEventReplay()
                {
                    public void replay(ChatRoomListener cb)
                    {
                        cb.leave(timestamp, name);
                    }
                });
                if(_replay.size() > MAX_MESSAGES)
                {
                    _replay.removeFirst();
                }

                _users.remove(name);

                copy = new ArrayList<ChatRoomListener>(_listeners);
            }

            for(ChatRoomListener listener : copy)
            {
                listener.leave(timestamp, name);
            }
        }

        public void send(final long timestamp, final String name, final String message, Ice.Current current)
        {
            List<ChatRoomListener> copy;

            synchronized(this)
            {
                _replay.add(new ChatEventReplay()
                {
                    public void replay(ChatRoomListener cb)
                    {
                        cb.send(timestamp, name, message);
                    }
                });
                if(_replay.size() > MAX_MESSAGES)
                {
                    _replay.removeFirst();
                }

                copy = new ArrayList<ChatRoomListener>(_listeners);
            }

            for(ChatRoomListener listener : copy)
            {
                listener.send(timestamp, name, message);
            }
        }

        public static final long serialVersionUID = 1;
    }

    //
    // Any exception destroys the session.
    //
    // Should only be called from the main thread.
    //
    private void destroyWithError(final String msg)
    {
        List<ChatRoomListener> copy;

        synchronized(this)
        {
            destroy();
            _error = msg;

            copy = new ArrayList<ChatRoomListener>(_listeners);
        }

        for(ChatRoomListener listener : copy)
        {
            listener.error();
        }
    }

    private String stack2string(Throwable e)
    {
        try
        {
            java.io.StringWriter sw = new java.io.StringWriter();
            java.io.PrintWriter pw = new java.io.PrintWriter(sw);
            e.printStackTrace(pw);
            return sw.toString();
        }
        catch(Exception e2)
        {
            return e.toString();
        }
    }
}
