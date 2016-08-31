// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef CHAT_ICE
#define CHAT_ICE


/**
 *
 * The Chat module defines types shared by definitions from
 * ChatSession.ice and PollingChat.ice.
 *
 **/
module Chat
{

/**
 *
 * The InvalidMessageException is raised when a user sends an invalid
 * message to the server. A message is considered invalid if the
 * message size exceeds the maximum message size.
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

};

#endif
