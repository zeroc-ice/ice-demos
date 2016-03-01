// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
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

	// Required to prevent compiler warnings with MSVC++
	ChatSessionI& operator=(const ChatSessionI&);

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
