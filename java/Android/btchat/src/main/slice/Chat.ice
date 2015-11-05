// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

#include <Ice/Identity.ice>

/**
 *
 * The Chat module defines the types that two devices use to communicate with one another.
 *
 **/
module Chat
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
 * Peer represents a participant in a chat.
 *
 **/
interface Peer
{
    /**
     *
     * Requests a chat session with a peer. Only the initiator of the chat needs to invoke
     * this operation.
     *
     * @param id The object identity of the local peer. The remote peer will use this identity
     * to create a bidirectional proxy for making callbacks to the local peer.
     *
     * @throws ConnectionException Raised if the remote peer rejects the connection request.
     *
     **/
    void connect(Ice::Identity id)
        throws ConnectionException;

    /**
     *
     * Sends a chat message to the remote peer.
     *
     * @param message The chat message, which may be subject to validation (length, content, etc).
     *
     * @throws InvalidMessageException Raised if the remote peer rejects the message.
     *
     **/
    void send(string message)
        throws InvalidMessageException;


    /**
     *
     * Notifies the remote peer that the chat session is terminating immediately.
     *
     **/
    void disconnect();
};

};
