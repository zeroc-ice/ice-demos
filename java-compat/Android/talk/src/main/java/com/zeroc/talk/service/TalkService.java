// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

package com.zeroc.talk.service;

import android.app.Service;
import android.content.Intent;
import android.os.Binder;
import android.os.Build.VERSION;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;

import com.zeroc.talk.TalkActivity;
import com.zeroc.talk.R;

public class TalkService extends Service implements com.zeroc.talk.service.Service
{
    private Handler _handler;
    private int _state;
    private Ice.Communicator _communicator;
    private Ice.ObjectAdapter _adapter;

    //
    // The well-known Bluetooth service UUIDs for the Ice talk service.
    //
    private final static String SERVICE_ID = "6a193943-1754-4869-8d0a-ddc5f9a2b294";
    private final static String SERVICE_NAME = "Ice Bluetooth Talk";
    private final static String SSL_SERVICE_ID = "043257a6-d67c-4000-aa62-2ffe4583d324";
    private final static String SSL_SERVICE_NAME = "Ice Bluetooth Talk (SSL)";

    //
    // PeerInfo tracks some state about a peer.
    //
    private static class PeerInfo
    {
        PeerInfo()
        {
        }

        PeerInfo(PeerInfo o)
        {
            address = o.address;
            name = o.name;
            peer = o.peer;
            servant = o.servant;
        }

        String getName()
        {
            return name != null ? name : address;
        }

        void removeServant(Ice.ObjectAdapter adapter)
        {
            if(servant != null)
            {
                try
                {
                    adapter.remove(servant);
                }
                catch(Ice.NotRegisteredException ex)
                {
                    // Ignore.
                }
            }
        }

        String address;
        String name;
        Talk.PeerPrx peer;
        Ice.Identity servant;
    }

    //
    // We use this servant to handle new incoming connections. We use a different servant implementation
    // for outgoing connections.
    //
    private class PeerImpl extends Talk._PeerDisp
    {
        @Override
        public void connect(Ice.Identity id, Ice.Current current)
            throws Talk.ConnectionException
        {
            info = incoming(id, current.con);
        }

        @Override
        public void send(String message, Ice.Current current)
        {
            received(info, message);
        }

        @Override
        public void disconnect(Ice.Current current)
        {
            disconnected(info);
        }

        PeerInfo info;
    }

    private PeerInfo _peer;

    public class LocalBinder extends Binder
    {
        public com.zeroc.talk.service.Service getService()
        {
            return TalkService.this;
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
        _state = STATE_NOT_CONNECTED;
    }

    @Override
    public void onDestroy()
    {
        super.onDestroy();

        try
        {
            _communicator.destroy();
        }
        catch(Ice.LocalException ex)
        {
        }

        _handler = null;
        _communicator = null;
        _adapter = null;
    }

    @Override
    synchronized public int onStartCommand(Intent intent, int flags, int startId)
    {
        return START_STICKY;
    }

    // Called only from the UI thread.
    public void setHandler(Handler handler)
    {
        if(_handler != null)
        {
            return;
        }

        _handler = handler;
    }

    // Called only from the UI thread.
    public void initialize()
    {
        if(_handler == null)
        {
            throw new AssertionError();
        }

        if(_communicator != null)
        {
            return;
        }

        try
        {
            Ice.InitializationData initData = new Ice.InitializationData();

            //
            // Install a dispatcher to ensure Ice calls are dispatched by the main thread.
            //
            initData.dispatcher = new Ice.Dispatcher()
            {
                @Override
                public void dispatch(Runnable runnable, Ice.Connection connection)
                {
                    _handler.post(runnable);
                }
            };

            initData.properties = Ice.Util.createProperties();

            //
            // You can enable network tracing by setting Ice.Trace.Network to 1, 2 or 3. The output
            // will be in logcat.
            //
            initData.properties.setProperty("Ice.Trace.Network", "3");

            //
            // The IceSSL plug-in is required for using SSL-over-Bluetooth.
            //
            initData.properties.setProperty("IceSSL.Trace.Security", "3");
            initData.properties.setProperty("IceSSL.KeystoreType", "BKS");
            initData.properties.setProperty("IceSSL.TruststoreType", "BKS");
            initData.properties.setProperty("IceSSL.Password", "password");
            initData.properties.setProperty("Ice.InitPlugins", "0");
            initData.properties.setProperty("Ice.Plugin.IceSSL", "IceSSL.PluginFactory");

            //
            // SDK versions < 21 only support TLSv1 with SSLEngine.
            //
            if(VERSION.SDK_INT < 21)
            {
                initData.properties.setProperty("IceSSL.Protocols", "tls1_0");
            }

            //
            // Install the IceBT transport.
            //
            initData.properties.setProperty("Ice.Plugin.IceBT", "IceBT.PluginFactory");

            //
            // Initialize the communicator.
            //
            _communicator = Ice.Util.initialize(initData);

            //
            // Complete initialization of the IceSSL plug-in.
            //
            // Be sure to pass the same input stream to the SSL plug-in for
            // both the keystore and the truststore. This makes startup a
            // little faster since the plugin will not initialize
            // two keystores.
            //
            IceSSL.Plugin plugin = (IceSSL.Plugin)_communicator.getPluginManager().getPlugin("IceSSL");
            java.io.InputStream certs = getResources().openRawResource(R.raw.client);
            plugin.setKeystoreStream(certs);
            plugin.setTruststoreStream(certs);
            _communicator.getPluginManager().initializePlugins();

            //
            // Create an object adapter for receiving callbacks and incoming connections. The Bluetooth
            // endpoint specifies the talk service's ID and name.
            //
            _adapter = _communicator.createObjectAdapterWithEndpoints("",
                    "bt -u " + SERVICE_ID + " --name \"" + SERVICE_NAME + "\":" +
                    "bts -u " + SSL_SERVICE_ID + " --name \"" + SSL_SERVICE_NAME + "\"");

            //
            // Install a servant for handling incoming connections from peers.
            //
            _adapter.add(new PeerImpl(), Ice.Util.stringToIdentity("peer"));
            _adapter.activate();
        }
        catch(Ice.LocalException ex)
        {
            ex.printStackTrace();

            if(_communicator != null)
            {
                try
                {
                    _communicator.destroy();
                }
                catch(Ice.LocalException e)
                {
                }
                _communicator = null;
                _adapter = null;
            }

            log("Error initializing Ice");

            _handler = null;
        }
    }

    public void connect(String address, String name, boolean ssl)
    {
        final PeerInfo info = new PeerInfo();

        info.address = address;
        info.name = name;

        //
        // Compose a stringified proxy for the peer. The identity of the remote object is "peer". The UUID in the
        // proxy is the well-known Bluetooth service ID for the Talk service. The peer's address must be enclosed
        // in quotes.
        //
        String proxy = "peer:";
        if(ssl)
        {
            proxy += "bts -a \"" + address + "\" -u " + SSL_SERVICE_ID;
        }
        else
        {
            proxy += "bt -a \"" + address + "\" -u " + SERVICE_ID;
        }
        //
        // Create a proxy for the peer. The identity of the remote object is "peer". The UUID in the
        // proxy is the well-known Bluetooth service ID for the Talk service.
        //
        info.peer = Talk.PeerPrxHelper.uncheckedCast(_communicator.stringToProxy(proxy));

        //
        // Register a unique servant with the OA for each outgoing connection. This servant receives
        // callbacks from the peer.
        //
        info.servant = Ice.Util.stringToIdentity(java.util.UUID.randomUUID().toString());

        _adapter.add(
            new Talk._PeerDisp()
            {
                public void connect(Ice.Identity id, Ice.Current current)
                    throws Talk.ConnectionException
                {
                    throw new Talk.ConnectionException("already connected");
                }

                public void send(String message, Ice.Current current)
                {
                    received(info, message);
                }

                public void disconnect(Ice.Current current)
                {
                    disconnected(info);
                }
            }, info.servant);

        PeerInfo oldInfo = null;

        synchronized(this)
        {
            if(_state == STATE_CONNECTED)
            {
                oldInfo = new PeerInfo(_peer);
                setState(STATE_NOT_CONNECTED); // Also notifies the activity.
            }
            _peer = info;
        }

        //
        // Disconnect from the current peer.
        //
        if(oldInfo != null)
        {
            disconnect(oldInfo);
        }

        connect(info);
    }

    synchronized public void disconnect()
    {
        PeerInfo oldInfo = null;

        if(_state == STATE_NOT_CONNECTED)
        {
            toast("Not connected");
            return;
        }
        else if(_state == STATE_CONNECTED)
        {
            oldInfo = _peer;
        }

        _peer = null;
        setState(STATE_NOT_CONNECTED);

        if(oldInfo != null)
        {
            disconnect(oldInfo);
        }
    }

    synchronized public void send(final String message)
    {
        if(_state != STATE_CONNECTED)
        {
            toast("Not connected");
            return;
        }

        final PeerInfo info = _peer;

        info.peer.begin_send(message, new Ice.Callback()
            {
                @Override
                public void completed(Ice.AsyncResult r)
                {
                    try
                    {
                        info.peer.end_send(r);
                        TalkService.this.sent(info, message);
                    }
                    catch(Ice.LocalException ex)
                    {
                        sendFailed(info, "Send failed: " + ex.getClass().getName(), true);
                        ex.printStackTrace();
                    }
                }
            });
    }

    synchronized public int getState()
    {
        return _state;
    }

    synchronized private void setState(int state)
    {
        _state = state;
        _handler.obtainMessage(MESSAGE_STATE_CHANGE, state, -1).sendToTarget();
    }

    synchronized private void toast(String text)
    {
        Message msg = _handler.obtainMessage(MESSAGE_TOAST);
        Bundle bundle = new Bundle();
        bundle.putString(KEY_TEXT, text);
        msg.setData(bundle);
        _handler.sendMessage(msg);
    }

    synchronized private void log(String text)
    {
        Message msg = _handler.obtainMessage(MESSAGE_LOG);
        Bundle bundle = new Bundle();
        bundle.putString(KEY_TEXT, ">> " + text);
        msg.setData(bundle);
        _handler.sendMessage(msg);
    }

    synchronized private void received(PeerInfo info, String text)
    {
        if(_state == STATE_CONNECTED && _peer == info)
        {
            Message msg = _handler.obtainMessage(MESSAGE_RECEIVED);
            Bundle bundle = new Bundle();
            bundle.putString(KEY_TEXT, "Peer: " + text);
            msg.setData(bundle);
            _handler.sendMessage(msg);
        }
    }

    synchronized private void sent(PeerInfo info, String text)
    {
        if(_state == STATE_CONNECTED && _peer == info)
        {
            Message msg = _handler.obtainMessage(MESSAGE_SENT);
            Bundle bundle = new Bundle();
            bundle.putString(KEY_TEXT, "Me: " + text);
            msg.setData(bundle);
            _handler.sendMessage(msg);
        }
    }

    synchronized private PeerInfo incoming(Ice.Identity id, Ice.Connection con)
        throws Talk.ConnectionException
    {
        //
        // Only accept a new incoming connection if we're idle.
        //
        if(_state != STATE_NOT_CONNECTED)
        {
            throw new Talk.ConnectionException("Peer is busy");
        }

        Ice.ConnectionInfo ci = con.getInfo();
        if(ci.underlying != null)
        {
            ci = ci.underlying;
        }
        IceBT.ConnectionInfo btci = (IceBT.ConnectionInfo)ci;

        PeerInfo info = new PeerInfo();
        info.peer = Talk.PeerPrxHelper.uncheckedCast(con.createProxy(id));
        info.address = btci.remoteAddress;
        info.servant = null;

        setState(STATE_CONNECTED); // Also notifies the activity.
        _peer = info;
        configureConnection(con, info);
        log("Connected to " + info.getName());

        return info;
    }

    synchronized private void connect(final PeerInfo info)
    {
        if(_peer != info)
        {
            //
            // Don't bother trying to connect if the activity has already begun another connection attempt.
            // Remove the previous servant to avoid spurious callbacks and give up.
            //
            info.removeServant(_adapter);
            return;
        }

        if(_state != STATE_NOT_CONNECTED)
        {
            throw new AssertionError();
        }
        setState(STATE_CONNECTING); // Also notifies the activity.

        info.peer.begin_connect(info.servant, new Ice.Callback()
            {
                @Override
                public void completed(Ice.AsyncResult r)
                {
                    try
                    {
                        info.peer.end_connect(r);
                        connected(info);
                    }
                    catch(Talk.ConnectionException ex)
                    {
                        connectFailed(info, "Connection to " + info.getName() + " failed: " + ex.reason);
                    }
                    catch(Ice.LocalException ex)
                    {
                        connectFailed(info, "Connection to " + info.getName() + " failed: " + ex.getClass().getName());
                        ex.printStackTrace();
                    }
                }
            });
    }

    private void disconnect(final PeerInfo oldInfo)
    {
        //
        // We are disconnecting from oldInfo.peer. Remove the old servant we created to receive callbacks
        // from the old peer. Any new incoming callbacks for that servant will be discarded.
        //
        oldInfo.removeServant(_adapter);

        //
        // Try to gracefully disconnect from the old peer.
        //
        oldInfo.peer.begin_disconnect(new Ice.Callback()
            {
                @Override
                public void completed(Ice.AsyncResult r)
                {
                    try
                    {
                        oldInfo.peer.end_disconnect(r);
                    }
                    catch(Ice.LocalException ex)
                    {
                        // Ignore.
                    }

                    closeConnection(oldInfo.peer);
                }
            });
    }

    synchronized private void connected(final PeerInfo info)
    {
        if(_peer == info)
        {
            setState(STATE_CONNECTED);

            //
            // Configure the connection for bidirectional connections.
            //
            Ice.Connection con = info.peer.ice_getCachedConnection();
            con.setAdapter(_adapter);
            configureConnection(con, info);
            log("Connected to " + info.getName());
        }
        else
        {
            disconnect(info);
        }
    }

    private void disconnected(PeerInfo info)
    {
        log("Peer is disconnecting");
        closeConnection(info.peer);
    }

    synchronized private void connectFailed(PeerInfo info, String message)
    {
        if(_peer == info)
        {
            setState(STATE_NOT_CONNECTED);
            _peer = null;
        }

        info.removeServant(_adapter);
        log(message);
    }

    synchronized private void sendFailed(PeerInfo info, String message, boolean fatal)
    {
        if(_peer == info)
        {
            log(message);
            if(fatal)
            {
                setState(STATE_NOT_CONNECTED);
                _peer = null;
            }
        }

        if(fatal)
        {
            info.removeServant(_adapter);
            closeConnection(info.peer);
        }
    }

    private void configureConnection(Ice.Connection con, final PeerInfo info)
    {
        //
        // Set a callback to detect connection closure and configure active connection management settings.
        //
        con.setCloseCallback(new Ice.CloseCallback()
            {
                @Override
                public void closed(Ice.Connection c)
                {
                    connectionClosed(info);
                }
            });
        con.setACM(new Ice.IntOptional(30), new Ice.Optional<Ice.ACMClose>(Ice.ACMClose.CloseOff),
                   new Ice.Optional<Ice.ACMHeartbeat>(Ice.ACMHeartbeat.HeartbeatAlways));
    }

    private void closeConnection(Talk.PeerPrx peer)
    {
        Ice.Connection con = peer.ice_getCachedConnection();
        if(con != null)
        {
            con.close(false); // Our connection callback will eventually be called.
        }
    }

    synchronized private void connectionClosed(PeerInfo info)
    {
        log("Connection closed to " + info.getName());

        if(_peer == info)
        {
            if(_state != STATE_NOT_CONNECTED)
            {
                setState(STATE_NOT_CONNECTED);
            }
            _peer = null;
        }

        info.removeServant(_adapter);
    }
}
