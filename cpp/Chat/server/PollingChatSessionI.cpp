// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <PollingChatSessionI.h>
#include <ChatUtils.h>

using namespace std;

class PollCallbackAdapter : public ChatRoomCallbackAdapter
{
public:

    virtual void
    init(const Ice::StringSeq& users)
    {
        IceUtil::Mutex::Lock sync(_mutex);
        _users = users;
    }

    virtual void
    send(const PollingChat::MessageEventPtr& e)
    {
        IceUtil::Mutex::Lock sync(_mutex);
        _updates.push_back(e);
    }

    virtual void
    join(const PollingChat::UserJoinedEventPtr& e)
    {
        IceUtil::Mutex::Lock sync(_mutex);
        _updates.push_back(e);
    }

    virtual void
    leave(const PollingChat::UserLeftEventPtr& e)
    {
        IceUtil::Mutex::Lock sync(_mutex);
        _updates.push_back(e);
    }

    Ice::StringSeq
    getInitialUsers()
    {
        IceUtil::Mutex::Lock sync(_mutex);
        Ice::StringSeq users;
        users.swap(_users);
        return users;
    }

    PollingChat::ChatRoomEventSeq
    getUpdates()
    {
        IceUtil::Mutex::Lock sync(_mutex);
        PollingChat::ChatRoomEventSeq updates;
        updates.swap(_updates);
        return updates;
    }

private:

    Ice::StringSeq _users;
    PollingChat::ChatRoomEventSeq _updates;
    IceUtil::Mutex _mutex;
};
typedef IceUtil::Handle<PollCallbackAdapter> PollCallbackAdapterPtr;

PollingChatSessionI::PollingChatSessionI(const ChatRoomPtr& chatRoom, const string& name, bool trace,
                                         const Ice::LoggerPtr& logger) :
    _chatRoom(chatRoom),
    _name(name),
    _timestamp(IceUtil::Time::now(IceUtil::Time::Monotonic)),
    _destroy(false),
    _callback(new PollCallbackAdapter),
    _trace(trace),
    _logger(logger)
{
    _chatRoom->join(name, _callback);
}

Ice::StringSeq
PollingChatSessionI::getInitialUsers(const Ice::Current&)
{
    IceUtil::Mutex::Lock sync(_mutex);
    if(_destroy)
    {
        if(_trace)
        {
            ostringstream os;
            os << "User '" << _name << "' requested initial users list but the session is already destroyed.";
            _logger->trace("info", os.str());
        }
        throw Ice::ObjectNotExistException(__FILE__, __LINE__);
    }
    return _callback->getInitialUsers();
}

PollingChat::ChatRoomEventSeq
PollingChatSessionI::getUpdates(const Ice::Current&)
{
    IceUtil::Mutex::Lock sync(_mutex);
    if(_destroy)
    {
        if(_trace)
        {
            ostringstream os;
            os << "User '" << _name << "' requested session updates list but the session is already destroyed.";
            _logger->trace("info", os.str());
        }
        throw Ice::ObjectNotExistException(__FILE__, __LINE__);
    }
    _timestamp = IceUtil::Time::now(IceUtil::Time::Monotonic);
    return _callback->getUpdates();
}

Ice::Long
PollingChatSessionI::send(const string& message, const Ice::Current&)
{
    IceUtil::Mutex::Lock sync(_mutex);
    if(_destroy)
    {
        if(_trace)
        {
            ostringstream os;
            os << "User '" << _name << "' tried to send a message but the session is already destroyed.";
            _logger->trace("info", os.str());
        }
        throw Ice::ObjectNotExistException(__FILE__, __LINE__);
    }
    string msg;
    try
    {
        msg = validateMessage(message);
    }
    catch(const string& reason)
    {
        if(_trace)
        {
            ostringstream os;
            os << "User '" << _name << "' sent an invalid message:\n" << reason;
            _logger->trace("info", os.str());
        }
        throw Chat::InvalidMessageException(reason);
    }
    return _chatRoom->send(_name, msg);
}

void
PollingChatSessionI::destroy(const Ice::Current& c)
{
    IceUtil::Mutex::Lock sync(_mutex);
    if(_destroy)
    {
        if(_trace)
        {
            ostringstream os;
            os << "User '" << _name << "' tried to destroy the session but the session is already destroyed.";
            _logger->trace("info", os.str());
        }
        throw Ice::ObjectNotExistException(__FILE__, __LINE__);
    }
    try
    {
        c.adapter->remove(c.id);
        _chatRoom->leave(_name);
    }
    catch(const Ice::ObjectAdapterDeactivatedException&)
    {
        // No need to clean up, the server is shutting down.
    }
    if(_trace)
    {
        ostringstream os;
        os << "Poll session for user '" << _name << "' destroyed.";
        _logger->trace("info", os.str());
    }
    _destroy = true;
}

IceUtil::Time
PollingChatSessionI::timestamp() const
{
    IceUtil::Mutex::Lock sync(_mutex);
    if(_destroy)
    {
        if(_trace)
        {
            ostringstream os;
            os << "User '" << _name << "' requested the session timestamp but the session is already destroyed.";
            _logger->trace("info", os.str());
        }
        throw Ice::ObjectNotExistException(__FILE__, __LINE__);
    }
    return _timestamp;
}
