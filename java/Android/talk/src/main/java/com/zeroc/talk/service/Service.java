// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

package com.zeroc.talk.service;

import android.os.Handler;

public interface Service
{
    public final int STATE_NOT_CONNECTED = 1;
    public final int STATE_CONNECTING = 2;
    public final int STATE_CONNECTED = 3;

    public final int MESSAGE_STATE_CHANGE = 1;
    public final int MESSAGE_RECEIVED = 2;
    public final int MESSAGE_SENT = 3;
    public final int MESSAGE_TOAST = 4;
    public final int MESSAGE_LOG = 5;

    public final String KEY_TEXT = "text";

    /**
     *
     * Set the handler that will receive events.
     *
     * @param handler The handler.
     *
     */
    void setHandler(Handler handler);

    /**
     *
     * Initialize the service. Must only be called when network services (e.g., Bluetooth) are enabled.
     * Subsequent calls are ignored.
     *
     */
    void initialize();

    /**
     *
     * Establish a connection with the given peer.
     *
     * @param address The device address of the peer.
     * @param name The device name of the peer (or null).
     * @param ssl True to use SSL over Bluetooth.
     *
     */
    void connect(String address, String name, boolean ssl);

    /**
     *
     * Disconnect from the current peer.
     *
     */
    void disconnect();

    /**
     *
     * Send a message to the peer.
     *
     * @param message The text to be sent.
     *
     */
    void send(String message);

    /**
     *
     * Get the current state.
     *
     * @return The current state.
     *
     */
    int getState();
}
