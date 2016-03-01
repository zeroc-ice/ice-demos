// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef POLLING_CHAT_ICE
#define POLLING_CHAT_ICE

#include <Ice/BuiltinSequences.ice>
#include <Chat.ice>

/**
 *
 * The PollingChat module define types and interfaces for polling clients
 * (PHP and Silverlight clients), which cannot use Glacier2 or callbacks.
 *
 **/
module PollingChat
{

/**
 *
 * ChatRoomEvent is an abstract base class used to model a union of
 * possible event types.
 *
 **/
class ChatRoomEvent
{
    /** The timestamp. */
    long timestamp;
    /** The name of the user. */
    string name;
};

/**
 *
 * A sequence of state changes in the chat room.
 *
 * @see ChatRoomEvent
 *
 **/
sequence<ChatRoomEvent> ChatRoomEventSeq;

/**
 *
 * This event is generated when a user joins the chat room.
 *
 * @see ChatRoomEvent
 *
 **/
class UserJoinedEvent extends ChatRoomEvent
{
};

/**
 *
 * This event is generated when a user leaves the chat room.
 *
 * @see ChatRoomEvent
 *
 **/
class UserLeftEvent extends ChatRoomEvent
{
};

/**
 *
 * This event is generated when a user sends a message in the chat
 * room.
 *
 * @see ChatRoomEvent
 *
 **/
class MessageEvent extends ChatRoomEvent
{
    /** The contents of the message. */
    string message;
};

/**
 *
 * PollingChatSession is the session interface for polling clients.
 *
 **/
interface PollingChatSession
{
    /**
     *
     * This operation returns a sequence of string with the names of
     * the users in chat when this user connects. This function must
     * be called when the session is created and before any call to
     * getUpdates.
     *
     * @return The sequence of user names.
     *
     **/
    Ice::StringSeq getInitialUsers();

    /**
     *
     * This operation returns a sequence of ChatRoomEvent with the
     * events that occured in the chat room since the last time the
     * client called getUpdates.
     *
     * @return The sequence of chat room events.
     *
     **/
    ChatRoomEventSeq getUpdates();

    /**
     *
     * Send a message to the chat room.
     *
     * @param message The message to be sent.
     *
     * @return The time at which the message is sent.
     *
     * @throws InvalidMessageException should the message be invalid.
     *
     **/
    long send(string message) throws Chat::InvalidMessageException;

    /**
     *
     * Destroy the session and leave the chat room.
     *
     **/
    void destroy();
};

/**
 *
 * The CannotCreateSessionException indicates that a session could not be
 * created, for example, because the user name contains an invalid character.
 *
 **/
exception CannotCreateSessionException
{
    string reason;
};

/**
 *
 * Factory interface to create PollingChatSession objects.
 *
 **/
interface PollingChatSessionFactory
{
    /**
     *
     * Create a new session and join the chat room.
     *
     **/
    PollingChatSession* create(string name, string password) throws CannotCreateSessionException;
};

};

#endif
