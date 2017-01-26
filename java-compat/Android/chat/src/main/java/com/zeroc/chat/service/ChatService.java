// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************
package com.zeroc.chat.service;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Intent;
import android.os.Binder;
import android.os.Handler;
import android.os.IBinder;

import com.zeroc.chat.ChatActivity;
import com.zeroc.chat.R;

import java.io.IOException;

public class ChatService extends Service implements com.zeroc.chat.service.Service
{
    private static final int CHATACTIVE_NOTIFICATION = 0;
    private AppSession _session = null;
    private boolean _confirmConnectionInProgress = false;
    private SessionListener _listener;
    private boolean _loginInProgress;
    private Handler _handler;
    private String _loginError;

    public class LocalBinder extends Binder
    {
        public com.zeroc.chat.service.Service getService()
        {
            return ChatService.this;
        }
    }

    @Override
    public IBinder onBind(Intent intent)
    {
        return new LocalBinder();
    }

    @Override
    public void onCreate()
    {
        super.onCreate();
        _handler = new Handler();
    }

    @Override
    public void onDestroy()
    {
        super.onDestroy();
        logout();
    }

    @Override
    synchronized public int onStartCommand(Intent intent, int flags, int startId)
    {
        return START_STICKY;
    }

    // Called only from the UI thread.
    synchronized public boolean setSessionListener(SessionListener cb)
    {
        _listener = cb;
        if(_listener == null)
        {
            return false;
        }
        if(_loginInProgress)
        {
            if(_confirmConnectionInProgress)
            {
                _listener.onConnectConfirm();
            }
            else
            {
                _listener.onLoginInProgress();
            }
        }
        else if(_session != null)
        {
            _listener.onLogin();
        }
        else if(_loginError != null)
        {
            _listener.onLoginError();
        }
        return _loginInProgress;
    }

    synchronized public void logout()
    {
        if(_session != null)
        {
            _session.destroy();
            _session = null;

            sessionDestroyed();
        }
    }

    synchronized public void login(final String username, final String password) {
        assert _session == null;
        assert !_loginInProgress;

        _loginError = null;
        _loginInProgress = true;

        try
        {
            _session = new AppSession(this, getResources(), _handler, username, password);
        }
        catch(IOException ex)
        {
            loginFailed();
        }
    }

    synchronized public String addChatRoomListener(ChatRoomListener listener, boolean replay) throws NoSessionException
    {
        if(_session == null)
        {
            throw new NoSessionException();
        }
        return _session.addChatRoomListener(listener, replay);
    }

    synchronized public void removeChatRoomListener(ChatRoomListener listener)
    {
        if(_session != null)
        {
            _session.removeChatRoomListener(listener);
        }
    }

    synchronized public void send(String message)
    {
        if(_session != null)
        {
            _session.send(message);
        }
    }

    synchronized public String getLoginError()
    {
        return _loginError;
    }

    synchronized public String getSessionError()
    {
        return _session != null ? _session.getError() : "";
    }

    synchronized public void loginFailed()
    {
        _loginError = _session.getError();
        _loginInProgress = false;
        _session = null;
        if(_listener != null)
        {
            final SessionListener listener = _listener;
            _handler.post(new Runnable()
            {
                public void run()
                {
                    listener.onLoginError();
                }
            });
        }
    }

    synchronized public void loginComplete()
    {
        _loginInProgress = false;

        // Display a notification that the user is logged in.
        Notification notification = new Notification.Builder(this)
                .setSmallIcon(R.drawable.stat_notify)
                .setContentText("Logged In")
                .setWhen(System.currentTimeMillis())
                .setContentTitle("You are logged into server")
                .setContentIntent(PendingIntent.getActivity(this, 0, new Intent(this, ChatActivity.class), 0))
                .build();
        NotificationManager n = (NotificationManager)getSystemService(NOTIFICATION_SERVICE);
        n.notify(CHATACTIVE_NOTIFICATION, notification);

        if(_listener != null)
        {
            final SessionListener listener = _listener;
            _handler.post(new Runnable()
            {
                public void run()
                {
                    listener.onLogin();
                }
            });
        }
    }

    private void sessionDestroyed()
    {
        NotificationManager n = (NotificationManager)getSystemService(NOTIFICATION_SERVICE);
        // Cancel the notification -- we use the same ID that we had used to
        // start it
        n.cancel(CHATACTIVE_NOTIFICATION);
    }
}
