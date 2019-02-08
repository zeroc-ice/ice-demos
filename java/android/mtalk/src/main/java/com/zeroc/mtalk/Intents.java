//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

package com.zeroc.mtalk;

public interface Intents
{
    /** No extra data **/
    public static final String ACTION_START_MULTICAST = "com.zeroc.mtalk.START_MULTICAST";
    /** No extra data **/
    public static final String ACTION_STOP_MULTICAST = "com.zeroc.mtalk.STOP_MULTICAST";
    /** No extra data **/
    public static final String ACTION_NETWORK_UP = "com.zeroc.mtalk.NETWORK_UP";
    /** No extra data **/
    public static final String ACTION_NETWORK_DOWN = "com.zeroc.mtalk.NETWORK_DOWN";

    /**
     * Extra string PEER_NAME.
     * Extra string PEER_PROXY.
     **/
    public static final String ACTION_FOUND_PEER = "com.zeroc.mtalk.FOUND_PEER";

    public static final String EXTRA_PEER_NAME = "peer_name";
    public static final String EXTRA_PEER_PROXY = "peer_proxy";

    /**
     * Extra string PEER_STATUS.
     **/
    public static final String ACTION_PEER_STATUS = "com.zeroc.mtalk.PEER_STATUS";

    public static final String EXTRA_PEER_STATUS = "peer_status";

    public static final String PEER_NOT_CONNECTED = "not_connected";
    public static final String PEER_CONNECTING = "connecting";
    public static final String PEER_CONNECTED = "connected";
}
