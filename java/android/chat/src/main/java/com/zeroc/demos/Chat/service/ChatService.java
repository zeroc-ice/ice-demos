//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

package com.zeroc.demos.Chat.service;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Intent;
import android.os.Binder;
import android.os.Handler;
import android.os.IBinder;
import android.os.Build;

import com.zeroc.demos.Chat.ChatActivity;
import com.zeroc.demos.Chat.R;

import java.io.IOException;

public class ChatService extends Service implements com.zeroc.demos.Chat.service.Service
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
        public com.zeroc.demos.Chat.service.Service getService()
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
        if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.O)
        {
            android.app.NotificationChannel channel =
                    new android.app.NotificationChannel(getString(R.string.app_name),
                                                        getString(R.string.app_name),
                                                        NotificationManager.IMPORTANCE_DEFAULT);
            channel.setDescription(getString(R.string.app_description));
            // Register the channel with the system; you can't change the importance
            // or other notification behaviors after this
            NotificationManager notificationManager = getSystemService(NotificationManager.class);
            notificationManager.createNotificationChannel(channel);
        }
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

    synchronized public void login(final String username, final String password)
    {
        if(com.zeroc.demos.Chat.BuildConfig.DEBUG)
        {
            if(_session != null)
            {
                throw new AssertionError();
            }

            if(_loginInProgress)
            {
                throw new AssertionError();
            }
        }

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

    synchronized public String addChatRoomListener(ChatRoomListener listener, boolean replay)
        throws NoSessionException
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

    @SuppressWarnings("deprecation")
    synchronized public void loginComplete()
    {
        _loginInProgress = false;

        NotificationManager n = (NotificationManager)getSystemService(NOTIFICATION_SERVICE);
        // Display a notification that the user is logged in.
        Notification.Builder builder;
        if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.O)
        {
            builder = new Notification.Builder(this, getString(R.string.app_name));
        }
        else
        {
            //
            // The Suppress deprecation warnings annotation avoids a warnings
            // when using the now deprecated Notification.Builder constructor
            //
            builder = new Notification.Builder(this);
        }

        Notification notification = builder.setSmallIcon(R.drawable.stat_notify)
                    .setContentText("Logged In")
                    .setWhen(System.currentTimeMillis())
                    .setContentTitle("You are logged into the server")
                    .setContentIntent(PendingIntent.getActivity(this, 0, new Intent(this, ChatActivity.class), 0))
                    .build();

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
        // Cancel the notification -- we use the same ID that we used to start it
        n.cancel(CHATACTIVE_NOTIFICATION);
    }
}
