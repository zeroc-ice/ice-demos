// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// This copy of Chat Demo is licensed to you under the terms described
// in the CHAT_DEMO_LICENSE file included in this distribution.
//
// **********************************************************************
 
#ifndef CHAT_SESSION_I_H
#define CHAT_SESSION_I_H

#include <ChatSession.h>
#include <ChatRoom.h>

class ChatSessionI : public Chat::ChatSession
{
public:

    ChatSessionI(const ChatRoomPtr&, const std::string&, bool trace, const Ice::LoggerPtr& logger);

    virtual void setCallback(const Chat::ChatRoomCallbackPrx&, const Ice::Current&);
    virtual Ice::Long send(const std::string&, const Ice::Current&);
    virtual void destroy(const Ice::Current&);

private:

    const ChatRoomPtr _chatRoom;
    const std::string _name;
    ChatRoomCallbackAdapterPtr _callback;
    bool _destroy;
    IceUtil::Mutex _mutex;
    const bool _trace;
    const Ice::LoggerPtr _logger;
};
typedef IceUtil::Handle<ChatSessionI> ChatSessionIPtr;

#endif
