//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

package com.zeroc.hello;

import android.app.Application;
import android.content.Context;
import android.net.wifi.WifiManager;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;

import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.Connection;
import com.zeroc.Ice.InitializationData;
import com.zeroc.Ice.InitializationException;
import com.zeroc.Ice.InvocationFuture;
import com.zeroc.Ice.LocalException;
import com.zeroc.Ice.ObjectPrx;
import com.zeroc.Ice.Properties;
import com.zeroc.Ice.Util;
import com.zeroc.demos.android.hello.Demo.HelloPrx;

import java.security.KeyStore;
import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.CompletableFuture;

import javax.net.ssl.KeyManagerFactory;
import javax.net.ssl.SSLContext;
import javax.net.ssl.TrustManagerFactory;

public class HelloApp extends Application
{
    static class MessageReady
    {
        MessageReady(Communicator c, LocalException e)
        {
            communicator = c;
            ex = e;
        }

        Communicator communicator;
        LocalException ex;
    }

    @Override
    public void onCreate()
    {
        super.onCreate();
        _uiHandler = new Handler(Looper.getMainLooper())
        {
            @Override
            public void handleMessage(Message m)
            {
                if(m.what == MSG_READY)
                {
                    MessageReady ready = (MessageReady)m.obj;
                    _initialized = true;
                    _communicator = ready.communicator;
                }
                else if(m.what == MSG_EXCEPTION || m.what == MSG_RESPONSE)
                {
                    _result = null;
                }

                Message copy = new Message();
                copy.copyFrom(m);

                if(_handler != null)
                {
                    _handler.sendMessage(copy);
                }
                else
                {
                    _queue.add(copy);
                }
            }
        };

        WifiManager wifiManager = (WifiManager)getApplicationContext().getSystemService(Context.WIFI_SERVICE);

        // On some devices, a multicast lock must be acquired otherwise multicast packets are discarded.
        WifiManager.MulticastLock lock = wifiManager.createMulticastLock("com.zeroc.hello");
        lock.acquire();

        // SSL initialization can take some time. To avoid blocking the
        // calling thread, we perform the initialization in a separate thread.
        new Thread(() -> {
            try
            {
                InitializationData initData = new InitializationData();

                initData.properties = new Properties();
                initData.properties.setProperty("Ice.Trace.Network", "3");

                initData.properties.setProperty("IceSSL.Trace.Security", "3");
                initData.properties.setProperty("Ice.Plugin.IceDiscovery", "com.zeroc.IceDiscovery.PluginFactory");
                initData.properties.setProperty("Ice.Default.Package", "com.zeroc.demos.android.hello");

                // Load the user KeyStore from client.bks application resource
                KeyStore keyStore = KeyStore.getInstance(KeyStore.getDefaultType());
                keyStore.load(getResources().openRawResource(R.raw.client), "password".toCharArray());

                // Create a key manager manager factory that uses the certificates from our KeyStore.
                KeyManagerFactory keyManagerFactory =
                    KeyManagerFactory.getInstance(KeyManagerFactory.getDefaultAlgorithm());
                keyManagerFactory.init(keyStore, "password".toCharArray());

                // Create a trust manager factory that uses the certificates from our KeyStore.
                TrustManagerFactory trustManagerFactory =
                    TrustManagerFactory.getInstance(TrustManagerFactory.getDefaultAlgorithm());
                trustManagerFactory.init(keyStore);

                // Create a SSLContext for TLS and initialize it with our custom trust manager factory.
                var sslContext = SSLContext.getInstance("TLS");
                sslContext.init(keyManagerFactory.getKeyManagers(), trustManagerFactory.getTrustManagers(), null);

                // Setup the SSLEngine factory used by secure (TLS) client connections.
                initData.clientSSLEngineFactory =
                    (String peerHost, int peerPort) -> sslContext.createSSLEngine(peerHost, peerPort);

                Communicator c = Util.initialize(initData);
                _uiHandler.sendMessage(Message.obtain(_uiHandler, MSG_READY, new MessageReady(c, null)));
            }
            catch(LocalException e)
            {
                _uiHandler.sendMessage(Message.obtain(_uiHandler, MSG_READY, new MessageReady(null, e)));
            }
            catch (Exception e)
            {
                _uiHandler.sendMessage(Message.obtain(
                    _uiHandler,
                    MSG_READY,
                    new MessageReady(null, new InitializationException("Communicator initialization failed", e))));
            }
        }).start();
    }

    /** Called when the application is stopping. */
    @Override
    public void onTerminate()
    {
        super.onTerminate();
        if(_communicator != null)
        {
            _communicator.destroy();
        }
    }

    void setHandler(Handler handler)
    {
        // Nothing to do in this case.
        if(_handler != handler)
        {
            _handler = handler;

            if(_handler != null)
            {
                if(!_initialized)
                {
                    _handler.sendMessage(_handler.obtainMessage(MSG_WAIT));
                }
                else
                {
                    // Send all the queued messages.
                    while(!_queue.isEmpty())
                    {
                        _handler.sendMessage(_queue.remove(0));
                    }
                }
            }
        }
    }

    void setHost(String host)
    {
        _host = host;
        _proxy = null;
    }

    void setUseDiscovery(boolean b)
    {
        _useDiscovery = b;
        _proxy = null;
    }

    void setTimeout(int timeout)
    {
        _timeout = timeout;
        _proxy = null;
    }

    void setDeliveryMode(DeliveryMode mode)
    {
        _mode = mode;
        _proxy = null;
    }

    void flush()
    {
        if(_proxy != null)
        {
            _proxy.ice_flushBatchRequestsAsync().whenComplete((result, ex) ->
                {
                    if(ex != null)
                    {
                        _uiHandler.sendMessage(_uiHandler.obtainMessage(MSG_EXCEPTION, ex));
                    }
                });
        }
    }

    //
    // Called for a batch proxy.
    //
    void shutdown()
    {
        try
        {
            updateProxy();
            assert(_proxy != null);

            _proxy.shutdownAsync().whenComplete((result, ex) ->
                {
                    if(ex != null)
                    {
                        _uiHandler.sendMessage(_uiHandler.obtainMessage(MSG_EXCEPTION, ex));
                    }
                });
        }
        catch(LocalException ex)
        {
            _uiHandler.sendMessage(_uiHandler.obtainMessage(MSG_EXCEPTION, ex));
        }

    }

    static class BooleanHolder
    {
        boolean value;
    }

    void shutdownAsync()
    {
        try
        {
            updateProxy();
            assert (_proxy != null);

            if(_result != null)
            {
                return;
            }

            _resultMode = _mode;
            final BooleanHolder response = new BooleanHolder();
            _uiHandler.sendMessage(_uiHandler.obtainMessage(MSG_SENDING));
            CompletableFuture<Void> r = _proxy.shutdownAsync();
            r.whenComplete((result, ex) ->
                {
                    // There is no ordering guarantee between sent, response/exception.
                    response.value = true;
                    if(ex != null)
                    {
                        _uiHandler.sendMessage(_uiHandler.obtainMessage(MSG_EXCEPTION, ex));
                    }
                    else
                    {
                        _uiHandler.sendMessage(_uiHandler.obtainMessage(MSG_RESPONSE));
                    }
                });

            _result = Util.getInvocationFuture(r);
            _result.whenSent((sentSynchronously, ex) ->
                {
                    if(ex == null)
                    {
                        if(_resultMode.isOneway())
                        {
                            _uiHandler.sendMessage(_uiHandler.obtainMessage(MSG_RESPONSE));
                        }
                        else if(!response.value)
                        {
                            _uiHandler.sendMessage(_uiHandler.obtainMessage(MSG_SENT, _resultMode));
                        }
                    }
                });
        }
        catch(LocalException ex)
        {
            _uiHandler.sendMessage(_uiHandler.obtainMessage(MSG_EXCEPTION, ex));
        }
    }

    //
    // Called for a batch proxy.
    //
    void sayHello(int delay)
    {
        try
        {
            updateProxy();
            if(_proxy == null || _result != null)
            {
                return;
            }

            _proxy.sayHelloAsync(delay).whenComplete((result, ex) ->
                {
                    if(ex != null)
                    {
                        _uiHandler.sendMessage(_uiHandler.obtainMessage(MSG_EXCEPTION, ex));
                    }
                });
        }
        catch(LocalException ex)
        {
            _uiHandler.sendMessage(_uiHandler.obtainMessage(MSG_EXCEPTION, ex));
        }
    }

    void sayHelloAsync(int delay)
    {
        try
        {
            updateProxy();
            assert(_proxy != null);

            if(_result != null)
            {
                return;
            }

            _resultMode = _mode;
            final BooleanHolder response = new BooleanHolder();
            _uiHandler.sendMessage(_uiHandler.obtainMessage(MSG_SENDING));
            CompletableFuture<Void> r = _proxy.sayHelloAsync(delay);
            r.whenComplete((result, ex) ->
                {
                    // There is no ordering guarantee between sent, response/exception.
                    response.value = true;
                    if(ex != null)
                    {
                        _uiHandler.sendMessage(_uiHandler.obtainMessage(MSG_EXCEPTION, ex));
                    }
                    else
                    {
                        _uiHandler.sendMessage(_uiHandler.obtainMessage(MSG_RESPONSE));
                    }
                });

            _result = Util.getInvocationFuture(r);
            _result.whenSent((sentSynchronously, ex) ->
                {
                    if(ex == null)
                    {
                        if(_resultMode.isOneway())
                        {
                            _uiHandler.sendMessage(_uiHandler.obtainMessage(MSG_RESPONSE));
                        }
                        else if(!response.value)
                        {
                            _uiHandler.sendMessage(_uiHandler.obtainMessage(MSG_SENT, _resultMode));
                        }
                    }
                });
        }
        catch(LocalException ex)
        {
            _uiHandler.sendMessage(_uiHandler.obtainMessage(MSG_EXCEPTION, ex));
        }
    }

    private void updateProxy()
    {
        if(_proxy != null)
        {
            return;
        }

        var prx = HelloPrx.createProxy(
            _communicator,
            _useDiscovery ? "hello" : "hello:tcp -h " + _host + " -p 10000:ssl -h " + _host + " -p 10001:udp -h " + _host  + " -p 10000");

        prx = switch (_mode) {
            case TWOWAY -> prx.ice_twoway();
            case TWOWAY_SECURE -> prx.ice_twoway().ice_secure(true);
            case ONEWAY -> prx.ice_oneway();
            case ONEWAY_BATCH -> prx.ice_batchOneway();
            case ONEWAY_SECURE -> prx.ice_oneway().ice_secure(true);
            case ONEWAY_SECURE_BATCH -> prx.ice_batchOneway().ice_secure(true);
            case DATAGRAM -> prx.ice_datagram();
            case DATAGRAM_BATCH -> prx.ice_batchDatagram();
        };

        if(_timeout != 0)
        {
            prx = prx.ice_invocationTimeout(_timeout);
        }
        _proxy = prx;
    }

    DeliveryMode getDeliveryMode()
    {
        return _mode;
    }

    public static final int MSG_WAIT = 0;
    public static final int MSG_READY = 1;
    public static final int MSG_EXCEPTION = 2;
    public static final int MSG_RESPONSE = 3;
    public static final int MSG_SENDING = 4;
    public static final int MSG_SENT = 5;

    private final List<Message> _queue = new LinkedList<>();
    private Handler _uiHandler;

    private boolean _initialized;
    private Communicator _communicator;
    private HelloPrx _proxy = null;

    // The current request if any.
    private InvocationFuture<?> _result;
    // The mode of the current request.
    private DeliveryMode _resultMode;

    private Handler _handler;

    // Proxy settings.
    private String _host;
    private boolean _useDiscovery;
    private int _timeout;
    private DeliveryMode _mode;
}
