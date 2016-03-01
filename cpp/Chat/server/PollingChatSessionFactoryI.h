// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef POLLING_CHAT_SESSION_FACTORY_I_H
#define POLLING_CHAT_SESSION_FACTORY_I_H

#include <Ice/Ice.h>

#include <PollingChat.h>
#include <PollingChatSessionI.h>
#include <ChatRoom.h>


class ReaperTask : public IceUtil::TimerTask
{
public:

    ReaperTask(int timeout, bool trace, const Ice::LoggerPtr& logger);
    virtual void runTimerTask();
    void add(const PollingChat::PollingChatSessionPrx&, const PollingChatSessionIPtr&);

private:

    IceUtil::Mutex _mutex;
    const IceUtil::Time _timeout;
    std::list<std::pair<PollingChat::PollingChatSessionPrx, PollingChatSessionIPtr> > _reapables;
    bool _trace;
    Ice::LoggerPtr _logger;
};
typedef IceUtil::Handle<ReaperTask> ReaperTaskPtr;

class PollingChatSessionFactoryI : public PollingChat::PollingChatSessionFactory
{
public:

    PollingChatSessionFactoryI(const ChatRoomPtr&, const ReaperTaskPtr& reaper, bool, const Ice::LoggerPtr&);

    virtual PollingChat::PollingChatSessionPrx create(const std::string&, const std::string&, const Ice::Current&);

private:

	// Required to prevent compiler warnings with MSVC++
	PollingChatSessionFactoryI& operator=(const PollingChatSessionFactoryI&);

    const ChatRoomPtr _chatRoom;
    const ReaperTaskPtr _reaper;
    const bool _trace;
    const Ice::LoggerPtr _logger;
};

#endif
