//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

package com.zeroc.mtalk.service;

import android.os.Handler;

public interface Service
{
    public final int MESSAGE_RECEIVED = 1;
    public final int MESSAGE_SENT = 2;
    public final int MESSAGE_TOAST = 3;
    public final int MESSAGE_LOG = 4;

    public final String KEY_TEXT = "text";

    /**
     *
     * Set the handler that will receive events.
     *
     * @param handler The handler.
     *
     **/
    void setHandler(Handler handler);

    /**
     *
     * Initialize the service. Must only be called when network services (e.g., Bluetooth) are enabled.
     * Subsequent calls are ignored.
     *
     **/
    void initialize();

    /**
     *
     * Establish a connection with the given peer.
     *
     * @param name The peer's name.
     * @param name The peer's stringified proxy.
     *
     **/
    void connect(String name, String proxy);

    /**
     *
     * Disconnect from the current peer.
     *
     **/
    void disconnect();

    /**
     *
     * Send a message to the peer.
     *
     * @param message The text to be sent.
     * @return True if the message was accepted, false otherwise.
     *
     **/
    boolean send(String message);

    /**
     *
     * Determines whether the necessary network connectivity is available.
     *
     * @return True if the network is available, false, otherwise.
     *
     **/
    boolean isNetworkAvailable();

    /**
     *
     * Sets the user name.
     *
     * @param name The user's screen name.
     *
     **/
    void setName(String name);
}
