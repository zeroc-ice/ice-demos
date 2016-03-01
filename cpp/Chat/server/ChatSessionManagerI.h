// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef CHAT_SESSION_MANAGER_I_H
#define CHAT_SESSION_MANAGER_I_H

#include <Glacier2/Session.h>

#include <string>

#include <ChatRoom.h>

class ChatSessionManagerI : public Glacier2::SessionManager
{
public:

    ChatSessionManagerI(const ChatRoomPtr&, bool trace, const Ice::LoggerPtr& logger);
    virtual Glacier2::SessionPrx create(const std::string&,
                                        const Glacier2::SessionControlPrx&,
                                        const Ice::Current&);

private:

	// Required to prevent compiler warnings with MSVC++
	ChatSessionManagerI& operator=(const ChatSessionManagerI&);

    const ChatRoomPtr _chatRoom;
    const bool _trace;
    const Ice::LoggerPtr _logger;
};

#endif
