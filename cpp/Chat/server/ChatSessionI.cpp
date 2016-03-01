// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <ChatSessionI.h>
#include <ChatUtils.h>

using namespace std;

class SessionCallbackAdapter : public ChatRoomCallbackAdapter
{
public:

    SessionCallbackAdapter(const Chat::ChatRoomCallbackPrx& callback, const Chat::ChatSessionPrx& session, bool trace,
                           const Ice::LoggerPtr& logger, const std::string& name) :
        _callback(callback),
        _session(session),
        _trace(trace),
        _logger(logger),
        _name(name)
    {
    }

    void init(const Ice::StringSeq& users)
    {
        try
        {
            _callback->begin_init(users, Chat::newCallback_ChatRoomCallback_init(this,
                                                 &SessionCallbackAdapter::success, &SessionCallbackAdapter::failure));
        }
        catch(const Ice::CommunicatorDestroyedException&)
        {
            // Ignored server is being shutdown
        }
    }

    void join(const PollingChat::UserJoinedEventPtr& e)
    {
        try
        {
            _callback->begin_join(e->timestamp, e->name, Chat::newCallback_ChatRoomCallback_join(this,
                                                 &SessionCallbackAdapter::success, &SessionCallbackAdapter::failure));
        }
        catch(const Ice::CommunicatorDestroyedException&)
        {
            // Ignored server is being shutdown
        }
    }

    void leave(const PollingChat::UserLeftEventPtr& e)
    {
        try
        {
            _callback->begin_leave(e->timestamp, e->name, Chat::newCallback_ChatRoomCallback_leave(this,
                                                &SessionCallbackAdapter::success, &SessionCallbackAdapter::failure));
        }
        catch(const Ice::CommunicatorDestroyedException&)
        {
            // Ignored server is being shutdown
        }
    }

    void send(const PollingChat::MessageEventPtr& e)
    {
        try
        {
            _callback->begin_send(e->timestamp, e->name, e->message, Chat::newCallback_ChatRoomCallback_send(this,
                                                &SessionCallbackAdapter::success, &SessionCallbackAdapter::failure));
        }
        catch(const Ice::CommunicatorDestroyedException&)
        {
            // Ignored server is being shutdown
        }
    }

    void success()
    {
    }

    void failure(const Ice::Exception&)
    {
        if(_trace)
        {
            ostringstream os;
            os << "Error sending request to user '" << _name << "'. The user's session will be destroyed.";
            _logger->trace("info", os.str());
        }
        try
        {
            _session->destroy(); // Collocated call.
        }
        catch(const Ice::LocalException&)
        {
        }
    }

private:

	// Required to prevent compiler warnings with MSVC++
	SessionCallbackAdapter& operator=(const SessionCallbackAdapter&);

    const Chat::ChatRoomCallbackPrx _callback;
    const Chat::ChatSessionPrx _session;
    const bool _trace;
    const Ice::LoggerPtr _logger;
    const std::string _name;
};

ChatSessionI::ChatSessionI(const ChatRoomPtr& chatRoom, const string& name, bool trace, const Ice::LoggerPtr& logger) :
    _chatRoom(chatRoom),
    _name(name),
    _destroy(false),
    _trace(trace),
    _logger(logger)
{
}

void
ChatSessionI::setCallback(const Chat::ChatRoomCallbackPrx& callback, const Ice::Current& c)
{
    IceUtil::Mutex::Lock sync(_mutex);
    if(_destroy)
    {
        if(_trace)
        {
            ostringstream os;
            os << "User '" << _name << "' tried to set the session callback but the session is already destroyed.";
            _logger->trace("info", os.str());
        }
        throw Ice::ObjectNotExistException(__FILE__, __LINE__);
    }

    if(_callback || !callback)
    {
        return;
    }

    Ice::Context ctx;
    ctx["_fwd"] = "o";
    _callback = new SessionCallbackAdapter(callback->ice_context(ctx),
                                           Chat::ChatSessionPrx::uncheckedCast(c.adapter->createProxy(c.id)),
                                           _trace, _logger, _name);
    _chatRoom->join(_name, _callback);
}

Ice::Long
ChatSessionI::send(const string& message, const Ice::Current&)
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
    if(!_callback)
    {
        if(_trace)
        {
            ostringstream os;
            os << "User '" << _name << "' tried to send a message without setting the callback.";
            _logger->trace("info", os.str());
        }
        throw Chat::InvalidMessageException("You cannot send messages until you join the chat room.");
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
ChatSessionI::destroy(const Ice::Current& c)
{
    IceUtil::Mutex::Lock sync(_mutex);
    if(_destroy)
    {
        if(_trace)
        {
            ostringstream os;
            os << "Trying to destroy the session for user '" << _name << "' but the session is already destroyed.";
            _logger->trace("info", os.str());
        }
        throw Ice::ObjectNotExistException(__FILE__, __LINE__);
    }
    try
    {
        c.adapter->remove(c.id);
        if(_callback == 0)
        {
            _chatRoom->unreserve(_name);
        }
        else
        {
            _chatRoom->leave(_name);
        }
    }
    catch(const Ice::ObjectAdapterDeactivatedException&)
    {
        // No need to clean up, the server is shutting down.
    }
    if(_trace)
    {
        ostringstream os;
        os << "Push session for user '" << _name << "' destroyed.";
        _logger->trace("info", os.str());
    }
    _destroy = true;
}
