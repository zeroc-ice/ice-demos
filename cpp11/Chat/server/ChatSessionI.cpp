// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <ChatSessionI.h>
#include <ChatUtils.h>

using namespace std;

class SessionCallbackAdapter : public ChatRoomCallbackAdapter,
                               public enable_shared_from_this<SessionCallbackAdapter>
{
public:

    SessionCallbackAdapter(const shared_ptr<Chat::ChatRoomCallbackPrx>& callback,
                           const shared_ptr<Chat::ChatSessionPrx>& session,
                           bool trace, const shared_ptr<Ice::Logger>& logger, const std::string& name) :
        _callback(callback),
        _session(session),
        _trace(trace),
        _logger(logger),
        _name(name)
    {
    }

    void init(Ice::StringSeq users) override
    {
        auto self = shared_from_this();
        try
        {
            _callback->initAsync(users, nullptr, [self](std::exception_ptr eptr) { self->failed(eptr); });
        }
        catch(const Ice::CommunicatorDestroyedException&)
        {
            // Ignored server is being shutdown
        }
    }

    void join(const shared_ptr<PollingChat::UserJoinedEvent>& e) override
    {
        auto self = shared_from_this();
        try
        {
            _callback->joinAsync(e->timestamp, e->name, nullptr, [self](exception_ptr eptr) { self->failed(eptr); });
        }
        catch(const Ice::CommunicatorDestroyedException&)
        {
            // Ignored server is being shutdown
        }
        }

    void leave(const shared_ptr<PollingChat::UserLeftEvent>& e) override
    {
        auto self = shared_from_this();
        try
        {
            _callback->leaveAsync(e->timestamp, e->name, nullptr, [self](exception_ptr eptr) { self->failed(eptr); });
        }
        catch(const Ice::CommunicatorDestroyedException&)
        {
            // Ignored server is being shutdown
        }
    }

    void send(const shared_ptr<PollingChat::MessageEvent>& e) override
    {
        auto self = shared_from_this();
        try
        {
            _callback->sendAsync(e->timestamp, e->name, e->message, nullptr, [self](exception_ptr eptr) { self->failed(eptr); });
        }
        catch(const Ice::CommunicatorDestroyedException&)
        {
            // Ignored server is being shutdown
        }
    }

    void failed(exception_ptr)
    {
        if(_trace)
        {
            Ice::Trace out(_logger, "info");
            out << "Error sending request to user '" << _name << "'. The user's session will be destroyed.";
        }
        try
        {
            _session->ice_endpoints(Ice::EndpointSeq())->destroy(); // Collocated call.
        }
        catch(const Ice::LocalException&)
        {
        }
    }

private:

    const shared_ptr<Chat::ChatRoomCallbackPrx> _callback;
    const shared_ptr<Chat::ChatSessionPrx> _session;
    const bool _trace;
    const shared_ptr<Ice::Logger> _logger;
    const string _name;
};

ChatSessionI::ChatSessionI(const shared_ptr<ChatRoom>& chatRoom, const string& name, bool trace, const shared_ptr<Ice::Logger>& logger) :
    _chatRoom(chatRoom),
    _name(name),
    _trace(trace),
    _logger(logger)
{
}

void
ChatSessionI::setCallback(shared_ptr<Chat::ChatRoomCallbackPrx> callback, const Ice::Current& current)
{
    lock_guard<mutex> sync(_mutex);
    if(_destroy)
    {
        if(_trace)
        {
            Ice::Trace out(_logger, "info");
            out << "User '" << _name << "' tried to set the session callback but the session is already destroyed.";
        }
        throw Ice::ObjectNotExistException(__FILE__, __LINE__);
    }

    if(_callback || !callback)
    {
        return;
    }

    Ice::Context ctx;
    ctx["_fwd"] = "o";
    _callback = make_shared<SessionCallbackAdapter>(
        callback->ice_context(ctx),
        Ice::uncheckedCast<Chat::ChatSessionPrx>(current.adapter->createProxy(current.id)),
        _trace, _logger, _name);
    _chatRoom->join(_name, _callback);
}

long long
ChatSessionI::send(string message, const Ice::Current&)
{
    lock_guard<mutex> sync(_mutex);
    if(_destroy)
    {
        if(_trace)
        {
            Ice::Trace out(_logger, "info");
            out << "User '" << _name << "' tried to send a message but the session is already destroyed.";
        }
        throw Ice::ObjectNotExistException(__FILE__, __LINE__);
    }
    if(!_callback)
    {
        if(_trace)
        {
            Ice::Trace out(_logger, "info");
            out << "User '" << _name << "' tried to send a message without setting the callback.";
        }
        throw Chat::InvalidMessageException("You cannot send messages until you join the chat room.");
    }
    string msg;
    try
    {
        msg = validateMessage(message);
    }
    catch(const exception& ex)
    {
        if(_trace)
        {
            Ice::Trace out(_logger, "info");
            out << "User '" << _name << "' sent an invalid message:\n" << ex;
        }
        throw Chat::InvalidMessageException(ex.what());
    }
    return _chatRoom->send(_name, move(msg));
}

void
ChatSessionI::destroy(const Ice::Current& current)
{
    lock_guard<mutex> sync(_mutex);
    if(_destroy)
    {
        if(_trace)
        {
            Ice::Trace out(_logger, "info");
            out << "Trying to destroy the session for user '" << _name << "' but the session is already destroyed.";
        }
        throw Ice::ObjectNotExistException(__FILE__, __LINE__);
    }
    try
    {
        current.adapter->remove(current.id);
        if(_callback)
        {
            _chatRoom->leave(_name);
        }
        else
        {
            _chatRoom->unreserve(_name);
        }
    }
    catch(const Ice::ObjectAdapterDeactivatedException&)
    {
        // No need to clean up, the server is shutting down.
    }
    if(_trace)
    {
        Ice::Trace out(_logger, "info");
        out << "Push session for user '" << _name << "' destroyed.";
    }
    _destroy = true;
}
