// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <PollingChatSessionFactoryI.h>
#include <PollingChatSessionI.h>
#include <ChatUtils.h>

using namespace std;

ReaperTask::ReaperTask(int timeout, bool trace, const Ice::LoggerPtr& logger) :
    _timeout(IceUtil::Time::seconds(timeout)),
    _trace(trace),
    _logger(logger)
{
}

void
ReaperTask::runTimerTask()
{
    IceUtil::Mutex::Lock sync(_mutex);
    std::list<std::pair<PollingChat::PollingChatSessionPrx,PollingChatSessionIPtr> >::iterator p = _reapables.begin();
    while(p != _reapables.end())
    {
        try
        {
            if((IceUtil::Time::now(IceUtil::Time::Monotonic) - p->second->timestamp()) > _timeout)
            {
                if(_trace)
                {
                    ostringstream os;
                    os << "Session: " << p->first << " reaped.";
                    _logger->trace("info", os.str());
                }
                p->first->destroy();
                p = _reapables.erase(p);
            }
            else
            {
                ++p;
            }
        }
        catch(const Ice::LocalException&)
        {
            p = _reapables.erase(p);
        }
    }
}

void
ReaperTask::add(const PollingChat::PollingChatSessionPrx& proxy, const PollingChatSessionIPtr& session)
{
    IceUtil::Mutex::Lock sync(_mutex);
    _reapables.push_back(make_pair(proxy, session));
}


PollingChatSessionFactoryI::PollingChatSessionFactoryI(const ChatRoomPtr& chatRoom, const ReaperTaskPtr& reaper,
                                                       bool trace, const Ice::LoggerPtr& logger) :
    _chatRoom(chatRoom),
    _reaper(reaper),
    _trace(trace),
    _logger(logger)
{
}

PollingChat::PollingChatSessionPrx
PollingChatSessionFactoryI::create(const string& name, const string&, const Ice::Current& c)
{
    string vname;
    try
    {
        vname = validateName(name);
        _chatRoom->reserve(vname);
    }
    catch(const string& reason)
    {
        if(_trace)
        {
            ostringstream os;
            os << "Cannot create poll session:\n" << reason;
            _logger->trace("info", os.str());
        }
        throw PollingChat::CannotCreateSessionException(reason);
    }

    PollingChat::PollingChatSessionPrx proxy;
    try
    {
        PollingChatSessionIPtr session = new PollingChatSessionI(_chatRoom, vname, _trace, _logger);
        proxy = PollingChat::PollingChatSessionPrx::uncheckedCast(c.adapter->addWithUUID(session));
        _reaper->add(proxy, session);
    }
    catch(const Ice::ObjectAdapterDeactivatedException&)
    {
        ostringstream os;
        os << "Cannot create poll session for user '" << vname << "', ChatServer adapter is deactivated.";
        _logger->trace("info", os.str());
        throw PollingChat::CannotCreateSessionException("internal server error");
    }

    if(_trace)
    {
        ostringstream os;
        os << "Poll session created for user '" << vname << "'.";
        _logger->trace("info", os.str());
    }
    return proxy;
}
