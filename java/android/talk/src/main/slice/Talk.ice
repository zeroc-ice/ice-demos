//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#pragma once

["java:package:com.zeroc.demos.android.talk"]
/**
 *
 * The Talk module defines the types that two devices use to communicate with one another.
 *
 **/
module Talk
{

/**
 *
 * ConnectionException describes a connection failure.
 *
 **/
exception ConnectionException
{
    /**
     *
     * The reason why the connection failed.
     *
     **/
    string reason;
}

/**
 *
 * The Peer interface represents one of the participants in a talk session.
 *
 **/
interface Peer
{
    /**
     *
     * Initiate a talk session with a remote peer. The remote peer should set up a
     * bidirectional connection and send its messages on that connection.
     *
     * @param peer The proxy of the initiator's Peer object.
     * @throws ConnectionException If the remote peer rejects the request. For example,
     * the remote peer may already be in a talk session.
     *
     **/
    void connect(Peer* peer)
        throws ConnectionException;

    /**
     *
     * Send a message to the remote peer.
     *
     * @param message The message to be sent.
     *
     **/
    void send(string message);

    /**
     *
     * Notify the remote peer that the local peer is disconnecting.
     *
     **/
    void disconnect();
}

}
