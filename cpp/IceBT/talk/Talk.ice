// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

#include <Ice/Identity.ice>

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
};

/**
 *
 * InvalidMessageException is raised when a user sends an invalid
 * message. A message is considered invalid if the message size exceeds
 * the maximum message size.
 *
 **/
exception InvalidMessageException
{
    /**
     *
     * The reason why the message was rejected by the server.
     *
     **/
    string reason;
};

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
     * @param id The object identity of the initiator's local Peer object.
     * @throws ConnectionException If the remote peer rejects the request. For example,
     * the remote peer may already be in a talk session.
     *
     **/
    void connect(Ice::Identity id)
        throws ConnectionException;

    /**
     *
     * Send a message to the remote peer.
     *
     * @param message The message to be sent.
     * @throws InvalidMessageException If the remote peer rejects the message. For example,
     * the message size may be too large.
     *
     **/
    void send(string message)
        throws InvalidMessageException;

    /**
     *
     * Notify the remote peer that the local peer is disconnecting.
     *
     **/
    void disconnect();
};

};
