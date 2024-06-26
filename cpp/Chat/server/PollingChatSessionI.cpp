//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include <ChatUtils.h>
#include <PollingChatSessionI.h>

using namespace std;

class PollCallbackAdapter final : public ChatRoomCallbackAdapter
{
public:
    void init(Ice::StringSeq users) final
    {
        const lock_guard<mutex> sync(_mutex);
        _users = std::move(users);
    }

    void send(const shared_ptr<PollingChat::MessageEvent>& e) final
    {
        const lock_guard<mutex> sync(_mutex);
        _updates.push_back(e);
    }

    void join(const shared_ptr<PollingChat::UserJoinedEvent>& e) final
    {
        const lock_guard<mutex> sync(_mutex);
        _updates.push_back(e);
    }

    void leave(const shared_ptr<PollingChat::UserLeftEvent>& e) final
    {
        const lock_guard<mutex> sync(_mutex);
        _updates.push_back(e);
    }

    Ice::StringSeq getInitialUsers()
    {
        const lock_guard<mutex> sync(_mutex);
        Ice::StringSeq users;
        users.swap(_users);
        return users;
    }

    PollingChat::ChatRoomEventSeq getUpdates()
    {
        const lock_guard<mutex> sync(_mutex);
        PollingChat::ChatRoomEventSeq updates;
        updates.swap(_updates);
        return updates;
    }

private:
    Ice::StringSeq _users;
    PollingChat::ChatRoomEventSeq _updates;
    mutex _mutex;
};

PollingChatSessionI::PollingChatSessionI(
    const shared_ptr<ChatRoom>& chatRoom,
    const string& name,
    bool trace,
    const shared_ptr<Ice::Logger>& logger)
    : _chatRoom(chatRoom),
      _name(name),
      _callback(make_shared<PollCallbackAdapter>()),
      _trace(trace),
      _logger(logger)
{
    _chatRoom->join(name, _callback);
}

Ice::StringSeq
PollingChatSessionI::getInitialUsers(const Ice::Current&)
{
    const lock_guard<mutex> sync(_mutex);
    if (_destroy)
    {
        if (_trace)
        {
            Ice::Trace out(_logger, "info");
            out << "User '" << _name << "' requested initial users list but the session is already destroyed.";
        }
        throw Ice::ObjectNotExistException(__FILE__, __LINE__);
    }
    return _callback->getInitialUsers();
}

PollingChat::ChatRoomEventSeq
PollingChatSessionI::getUpdates(const Ice::Current&)
{
    const lock_guard<mutex> sync(_mutex);
    if (_destroy)
    {
        if (_trace)
        {
            Ice::Trace out(_logger, "info");
            out << "User '" << _name << "' requested session updates list but the session is already destroyed.";
        }
        throw Ice::ObjectNotExistException(__FILE__, __LINE__);
    }
    return _callback->getUpdates();
}

int64_t
PollingChatSessionI::send(string message, const Ice::Current&)
{
    const lock_guard<mutex> sync(_mutex);
    if (_destroy)
    {
        if (_trace)
        {
            Ice::Trace out(_logger, "info");
            out << "User '" << _name << "' tried to send a message but the session is already destroyed.";
        }
        throw Ice::ObjectNotExistException(__FILE__, __LINE__);
    }
    string msg;
    try
    {
        msg = validateMessage(message);
    }
    catch (const exception& ex)
    {
        if (_trace)
        {
            Ice::Trace out(_logger, "info");
            out << "User '" << _name << "' sent an invalid message:\n" << ex;
        }
        throw Chat::InvalidMessageException(ex.what());
    }
    return _chatRoom->send(_name, msg);
}

void
PollingChatSessionI::destroy(const Ice::Current& current)
{
    const lock_guard<mutex> sync(_mutex);
    if (_destroy)
    {
        if (_trace)
        {
            Ice::Trace out(_logger, "info");
            out << "User '" << _name << "' tried to destroy the session but the session is already destroyed.";
        }
        throw Ice::ObjectNotExistException(__FILE__, __LINE__);
    }
    try
    {
        current.adapter->remove(current.id);
        _chatRoom->leave(_name);
    }
    catch (const Ice::ObjectAdapterDeactivatedException&)
    {
        // No need to clean up, the server is shutting down.
    }
    if (_trace)
    {
        Ice::Trace out(_logger, "info");
        out << "Poll session for user '" << _name << "' destroyed.";
    }
    _destroy = true;
}
