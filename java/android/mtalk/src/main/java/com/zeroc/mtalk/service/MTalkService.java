//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

package com.zeroc.mtalk.service;

import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.ConnectivityManager;
import android.net.wifi.WifiManager;
import android.os.Binder;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;

import java.math.BigInteger;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.nio.ByteOrder;

import com.zeroc.demos.android.mtalk.MTalk.*;
import com.zeroc.Ice.ACMClose;
import com.zeroc.Ice.ACMHeartbeat;
import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.Connection;
import com.zeroc.Ice.ConnectionClose;
import com.zeroc.Ice.Current;
import com.zeroc.Ice.InitializationData;
import com.zeroc.Ice.LocalException;
import com.zeroc.Ice.NotRegisteredException;
import com.zeroc.Ice.ObjectAdapter;
import com.zeroc.Ice.Util;
import com.zeroc.mtalk.BuildConfig;
import com.zeroc.mtalk.Intents;
import com.zeroc.mtalk.R;

public class MTalkService extends Service implements com.zeroc.mtalk.service.Service
{
    private WifiManager _wifiManager;
    private WifiManager.MulticastLock _lock;
    private Handler _handler;
    private String _state;
    private Communicator _communicator;
    private boolean _networkEnabled;
    private ObjectAdapter _adapter;
    private ObjectAdapter _multicastAdapter;
    private PeerPrx _local;
    private DiscoveryPrx _discovery;
    private String _name;

    private final static String MULTICAST_ENDPOINT = "udp -h 239.255.1.1 -p 10000";
    private final static int MULTICAST_FREQUENCY = 2000; // ms

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
            name = o.name;
            remote = o.remote;
            local = o.local;
            connection = o.connection;
        }

        String getName()
        {
            return name;
        }

        void removeServant(ObjectAdapter adapter)
        {
            if(local != null)
            {
                try
                {
                    adapter.remove(local.ice_getIdentity());
                }
                catch(NotRegisteredException ex)
                {
                    // Ignore.
                }
            }
        }

        String name;
        PeerPrx remote;
        PeerPrx local;
        Connection connection;
    }

    //
    // We use this servant to handle new incoming connections. We use a different servant implementation
    // for outgoing connections.
    //
    private class PeerImpl implements Peer
    {
        @Override
        public void connect(String name, PeerPrx peer, Current current)
            throws ConnectionException
        {
            info = incoming(name, peer.ice_fixed(current.con), current.con);
        }

        @Override
        public void message(String text, Current current)
        {
            received(info, text);
        }

        @Override
        public void disconnect(Current current)
        {
            disconnected(info);
        }

        PeerInfo info;
    }

    //
    // This servant receives multicast messages.
    //
    private class DiscoveryImpl implements Discovery
    {
        @Override
        public void announce(String name, PeerPrx proxy, Current current)
        {
            discoveredPeer(name, proxy);
        }
    }

    private PeerInfo _peer;

    public class LocalBinder extends Binder
    {
        public com.zeroc.mtalk.service.Service getService()
        {
            return MTalkService.this;
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
        _state = Intents.PEER_NOT_CONNECTED;
        _name = Build.MODEL;

        Context context = getApplicationContext();
        _wifiManager = (WifiManager)context.getSystemService(Context.WIFI_SERVICE);

        //
        // On some devices, a multicast lock must be acquired otherwise multicast packets are discarded.
        // The lock is initially unacquired.
        //
        _lock = _wifiManager.createMulticastLock("com.zeroc.mtalk");

        //
        // Listen for requests to start and stop multicast.
        //
        IntentFilter filter = new IntentFilter();
        filter.addAction(Intents.ACTION_START_MULTICAST);
        filter.addAction(Intents.ACTION_STOP_MULTICAST);
        this.registerReceiver(_receiver, filter);

        //
        // Listen for changes to network status.
        //
        ConnectivityManager connectivityManager = (ConnectivityManager)context.getSystemService(Context.CONNECTIVITY_SERVICE);
        connectivityManager.registerDefaultNetworkCallback(new ConnectivityManager.NetworkCallback()
            {
                @Override
                public void onAvailable(android.net.Network network)
                {
                    checkNetwork();
                }

                @Override
                public void onLost(android.net.Network network)
                {
                    checkNetwork();
                }

                @Override
                public void onUnavailable()
                {
                    checkNetwork();
                }
            });
    }

    @Override
    public void onDestroy()
    {
        super.onDestroy();

        _communicator.destroy();

        this.unregisterReceiver(_receiver);

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
            InitializationData initData = new InitializationData();

            //
            // Install a dispatcher to ensure Ice calls are dispatched by the main thread.
            //
            initData.dispatcher = (Runnable runnable, Connection connection) ->
                {
                    _handler.post(runnable);
                };

            initData.properties = Util.createProperties();

            initData.properties.setProperty("Ice.Default.Timeout", "5000");

            //
            // You can enable network tracing by setting Ice.Trace.Network to 1, 2 or 3. The output
            // will be in logcat.
            //
            initData.properties.setProperty("Ice.Trace.Network", "3");

            //
            // Install and configure the IceSSL plug-in.
            //
            initData.properties.setProperty("IceSSL.Trace.Security", "3");
            initData.properties.setProperty("IceSSL.KeystoreType", "BKS");
            initData.properties.setProperty("IceSSL.TruststoreType", "BKS");
            initData.properties.setProperty("IceSSL.Password", "password");
            initData.properties.setProperty("Ice.Plugin.IceSSL", "com.zeroc.IceSSL.PluginFactory");
            initData.properties.setProperty("Ice.Default.Package", "com.zeroc.demos.android.mtalk");

            //
            // We need to postpone plug-in initialization so that we can configure IceSSL
            // with a resource stream for the certificate information.
            //
            initData.properties.setProperty("Ice.InitPlugins", "0");

            //
            // Initialize the communicator.
            //
            _communicator = Util.initialize(initData);

            //
            // Complete initialization of the IceSSL plug-in.
            //
            // Be sure to pass the same input stream to the SSL plug-in for both the keystore and the truststore.
            // This makes startup a little faster since the plug-in will not initialize two keystores.
            //
            com.zeroc.IceSSL.Plugin plugin =
                (com.zeroc.IceSSL.Plugin)_communicator.getPluginManager().getPlugin("IceSSL");
            java.io.InputStream certs = getResources().openRawResource(R.raw.client);
            plugin.setKeystoreStream(certs);
            plugin.setTruststoreStream(certs);
            _communicator.getPluginManager().initializePlugins();

            //
            // Construct a datagram proxy that we'll use for multicast.
            //
            // Note that we need to disable collocation optimization because we also create an object adapter with
            // the same endpoint and we always want our discovery announcements to be broadcast on the network.
            //
            _discovery = DiscoveryPrx.uncheckedCast(
                _communicator.stringToProxy("discover:" + MULTICAST_ENDPOINT)
                    .ice_datagram().ice_collocationOptimized(false));

            checkNetwork();
        }
        catch(LocalException ex)
        {
            ex.printStackTrace();

            if(_communicator != null)
            {
                _communicator.destroy();
                _communicator = null;
                _adapter = null;
            }

            log("Error initializing Ice");

            _handler = null;
        }
    }

    public void connect(String name, String proxy)
    {
        final PeerInfo info = new PeerInfo();

        info.name = name;
        info.remote = PeerPrx.uncheckedCast(
            _communicator.stringToProxy(proxy).ice_invocationTimeout(5000));

        //
        // Register a unique servant with the OA for each outgoing connection. This servant receives
        // callbacks from the peer.
        //
        info.local = PeerPrx.uncheckedCast(_adapter.addWithUUID(
            new Peer()
            {
                public void connect(String name, PeerPrx peer, Current current)
                    throws ConnectionException
                {
                    throw new ConnectionException("already connected");
                }

                public void message(String message, Current current)
                {
                    received(info, message);
                }

                public void disconnect(Current current)
                {
                    disconnected(info);
                }
            }));

        PeerInfo oldInfo = null;

        synchronized(this)
        {
            if(_state.equals(Intents.PEER_CONNECTED))
            {
                oldInfo = new PeerInfo(_peer);
                setState(Intents.PEER_NOT_CONNECTED); // Also notifies the activity.
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

        if(_state.equals(Intents.PEER_NOT_CONNECTED))
        {
            toast("Not connected");
            return;
        }
        else if(_state.equals(Intents.PEER_CONNECTED))
        {
            oldInfo = _peer;
        }

        _peer = null;
        setState(Intents.PEER_NOT_CONNECTED);

        if(oldInfo != null)
        {
            disconnect(oldInfo);
        }
    }

    synchronized public boolean send(final String message)
    {
        if(!_state.equals(Intents.PEER_CONNECTED))
        {
            toast("Not connected");
            return false;
        }

        final PeerInfo info = _peer;

        info.remote.messageAsync(message).whenComplete((result, ex) ->
            {
                if(ex != null)
                {
                    sendFailed(info, "Send failed: " + ex.getClass().getName(), true);
                    ex.printStackTrace();
                }
                else
                {
                    sent(info, message);
                }
            });

        return true;
    }

    synchronized public boolean isNetworkAvailable()
    {
        return _networkEnabled;
    }

    synchronized public void setName(String name)
    {
        _name = name;
    }

    synchronized private void setState(String state)
    {
        _state = state;
        Intent intent = new Intent(Intents.ACTION_PEER_STATUS);
        intent.putExtra(Intents.EXTRA_PEER_STATUS, state);
        sendBroadcast(intent);
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
        if(_state.equals(Intents.PEER_CONNECTED) && _peer == info)
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
        if(_state.equals(Intents.PEER_CONNECTED) && _peer == info)
        {
            Message msg = _handler.obtainMessage(MESSAGE_SENT);
            Bundle bundle = new Bundle();
            bundle.putString(KEY_TEXT, "Me: " + text);
            msg.setData(bundle);
            _handler.sendMessage(msg);
        }
    }

    synchronized private PeerInfo incoming(String name, PeerPrx peer, com.zeroc.Ice.Connection con)
        throws ConnectionException
    {
        //
        // Only accept a new incoming connection if we're idle.
        //
        if(!_state.equals(Intents.PEER_NOT_CONNECTED))
        {
            throw new ConnectionException("Peer is busy");
        }

        PeerInfo info = new PeerInfo();
        info.name = name;
        info.remote = peer.ice_invocationTimeout(5000);
        info.local = null;
        info.connection = con;

        setState(Intents.PEER_CONNECTED); // Also notifies the activity.
        _peer = info;
        configureConnection(info);
        log("Connected to " + name);

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

        if(!_state.equals(Intents.PEER_NOT_CONNECTED))
        {
            throw new AssertionError();
        }
        setState(Intents.PEER_CONNECTING); // Also notifies the activity.

        info.remote.connectAsync(_name, info.local).whenComplete((result, ex) ->
            {
                if(ex != null)
                {
                    if(ex instanceof ConnectionException)
                    {
                        connectFailed(info, "Connection to " + info.getName() + " failed: " +
                                      ((ConnectionException)ex).reason);
                    }
                    else
                    {
                        connectFailed(info, "Connection to " + info.getName() + " failed: " + ex.getClass().getName());
                        ex.printStackTrace();
                    }
                }
                else
                {
                    connected(info);
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
        oldInfo.remote.disconnectAsync().whenComplete((result, ex) ->
            {
                if(oldInfo.connection != null)
                {
                    oldInfo.connection.close(ConnectionClose.Gracefully);
                }
            });
    }

    synchronized private void connected(final PeerInfo info)
    {
        if(_peer == info)
        {
            setState(Intents.PEER_CONNECTED);

            //
            // Configure the connection for bidirectional connections.
            //
            info.connection = info.remote.ice_getCachedConnection();
            info.connection.setAdapter(_adapter);
            configureConnection(info);
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
        if(info.connection != null)
        {
            info.connection.close(ConnectionClose.Gracefully);
        }
    }

    synchronized private void connectFailed(PeerInfo info, String message)
    {
        if(_peer == info)
        {
            setState(Intents.PEER_NOT_CONNECTED);
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
                setState(Intents.PEER_NOT_CONNECTED);
                _peer = null;
            }
        }

        if(fatal)
        {
            info.removeServant(_adapter);
            info.connection.close(ConnectionClose.Gracefully);
        }
    }

    private void configureConnection(final PeerInfo info)
    {
        //
        // Set a callback to detect connection closure and configure active connection management settings.
        //
        info.connection.setCloseCallback((con) ->
            {
                connectionClosed(info);
            });
        info.connection.setACM(java.util.OptionalInt.of(30), java.util.Optional.of(ACMClose.CloseOff),
                               java.util.Optional.of(ACMHeartbeat.HeartbeatAlways));
    }

    synchronized private void connectionClosed(PeerInfo info)
    {
        log("Connection closed to " + info.getName());

        if(_peer == info)
        {
            if(!_state.equals(Intents.PEER_NOT_CONNECTED))
            {
                setState(Intents.PEER_NOT_CONNECTED);
            }
            _peer = null;
        }

        info.removeServant(_adapter);
    }

    private synchronized void discoveredPeer(String name, PeerPrx proxy)
    {
        if(!_name.equals(name)) // Ignore our own broadcasts.
        {
            Intent intent = new Intent(Intents.ACTION_FOUND_PEER);
            intent.putExtra(Intents.EXTRA_PEER_NAME, name);
            intent.putExtra(Intents.EXTRA_PEER_PROXY, proxy.toString());
            sendBroadcast(intent);
        }
    }

    synchronized private void startMulticast()
    {
        if(_networkEnabled)
        {
            if(BuildConfig.DEBUG && _lock.isHeld())
            {
                throw new AssertionError();
            }
            _lock.acquire();

            _handler.postDelayed(new Runnable()
            {
                @Override
                public void run()
                {
                    announce();

                    if(_lock.isHeld())
                    {
                        _handler.postDelayed(this, MULTICAST_FREQUENCY);
                    }
                }
            }, MULTICAST_FREQUENCY);
        }
    }

    synchronized private void stopMulticast()
    {
        if(_lock.isHeld())
        {
            _lock.release();
        }
    }

    private void announce()
    {
        try
        {
            _discovery.announce(_name, _local);
        }
        catch(Exception ex)
        {
            ex.printStackTrace();
            log("Error sending broadcast");
        }
    }

    private void checkNetwork()
    {
        ObjectAdapter adapter = null;
        ObjectAdapter multicastAdapter = null;

        synchronized(this)
        {
            if(_communicator == null)
            {
                //
                // Not initialized yet.
                //
                return;
            }

            final boolean oldValue = _networkEnabled;

            if(_wifiManager.isWifiEnabled())
            {
                _networkEnabled = true;
            }
            else
            {
                _networkEnabled = false;
            }

            if(oldValue != _networkEnabled)
            {
                adapter = _adapter;
                _adapter = null;
                multicastAdapter = _multicastAdapter;
                _multicastAdapter = null;

                if(_networkEnabled)
                {
                    try
                    {
                        //
                        // Create new object adapters. We need to specify an explicit IP address for the SSL
                        // endpoint, otherwise we might publish endpoints that aren't reachable.
                        //
                        String ip = getIpAddress();
                        if(ip == null)
                        {
                            log("Unable to get IP address");
                            _networkEnabled = false;
                        }
                        else
                        {
                            _adapter = _communicator.createObjectAdapterWithEndpoints("", "ssl -h \"" + ip + "\"");
                            _local = PeerPrx.uncheckedCast(
                                _adapter.add(new PeerImpl(), Util.stringToIdentity("peer")));
                            _adapter.activate();

                            _multicastAdapter = _communicator.createObjectAdapterWithEndpoints("", MULTICAST_ENDPOINT);
                            _multicastAdapter.add(new DiscoveryImpl(), Util.stringToIdentity("discover"));
                            _multicastAdapter.activate();
                        }
                    }
                    catch(LocalException ex)
                    {
                        ex.printStackTrace();
                        log("Error creating object adapters");
                        _networkEnabled = false;
                    }
                }
                else
                {
                    stopMulticast();
                }

                sendBroadcast(new Intent(_networkEnabled ? Intents.ACTION_NETWORK_UP : Intents.ACTION_NETWORK_DOWN));
            }
        }

        if(adapter != null)
        {
            adapter.destroy();
        }
        if(multicastAdapter != null)
        {
            multicastAdapter.destroy();
        }
    }

    private String getIpAddress()
    {
        int addr = _wifiManager.getConnectionInfo().getIpAddress();

        if(ByteOrder.nativeOrder().equals(ByteOrder.LITTLE_ENDIAN))
        {
            addr = Integer.reverseBytes(addr);
        }

        byte[] arr = BigInteger.valueOf(addr).toByteArray();

        String str;
        try
        {
            str = InetAddress.getByAddress(arr).getHostAddress();
        }
        catch(UnknownHostException ex)
        {
            str = null;
        }

        return str;
    }

    private final BroadcastReceiver _receiver = new BroadcastReceiver()
    {
        @Override
        public void onReceive(Context context, Intent intent)
        {
            String action = intent.getAction();

            if(Intents.ACTION_START_MULTICAST.equals(action))
            {
                startMulticast();
            }
            else if(Intents.ACTION_STOP_MULTICAST.equals(action))
            {
                stopMulticast();
            }
        }
    };
}
