//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#pragma once

["java:package:com.zeroc.demos.android.mtalk"]
/**
 *
 * The MTalk module defines the types that devices use to discover and communicate with one another.
 *
 **/
module MTalk
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
 * The Peer interface represents one of the participants in a conversation.
 *
 **/
interface Peer
{
    /**
     *
     * Initiate a conversation with a remote peer. The remote peer should set up a
     * bidirectional connection and send its messages on that connection.
     *
     * @param name The peer's descriptive name.
     * @param peer The proxy of the initiator's Peer object.
     * @throws ConnectionException If the remote peer rejects the request. For example,
     * the remote peer may already be in a conversation.
     *
     **/
    void connect(string name, Peer* peer)
        throws ConnectionException;

    /**
     *
     * Send a message to the remote peer.
     *
     * @param text The message to be sent.
     *
     **/
    void message(string text);

    /**
     *
     * Notify the remote peer that the local peer is disconnecting.
     *
     **/
    void disconnect();
}

/**
 *
 * The Discovery interface allows a peer to announce its presence.
 *
 **/
interface Discovery
{
    /**
     *
     * Announces that a peer is available.
     *
     * @param name The peer's user-readable name.
     * @param peer The proxy with which to connect to the peer.
     *
     **/
    void announce(string name, Peer* peer);
}

}
