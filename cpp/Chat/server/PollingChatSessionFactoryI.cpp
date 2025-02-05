// Copyright (c) ZeroC, Inc.

#include "ChatUtils.h"
#include "PollingChatSessionFactoryI.h"
#include "PollingChatSessionI.h"

#include <Ice/Ice.h>

using namespace std;

PollingChatSessionFactoryI::PollingChatSessionFactoryI(
    const shared_ptr<ChatRoom>& chatRoom,
    bool trace,
    const shared_ptr<Ice::Logger>& logger)
    : _chatRoom(chatRoom),
      _trace(trace),
      _logger(logger)
{
}

optional<PollingChat::PollingChatSessionPrx>
PollingChatSessionFactoryI::create(string name, string, const Ice::Current& current)
{
    string vname;
    try
    {
        vname = validateName(name);
        _chatRoom->reserve(vname);
    }
    catch (const exception& ex)
    {
        if (_trace)
        {
            Ice::Trace out(_logger, "info");
            out << "Cannot create poll session:\n" << ex;
        }
        throw PollingChat::CannotCreateSessionException(ex.what());
    }

    optional<PollingChat::PollingChatSessionPrx> proxy;

    auto session = make_shared<PollingChatSessionI>(_chatRoom, vname, _trace, _logger);
    proxy = Ice::uncheckedCast<PollingChat::PollingChatSessionPrx>(current.adapter->addWithUUID(session));

    auto collocProxy = proxy->ice_endpoints(Ice::EndpointSeq());

    auto trace = _trace;
    auto logger = _logger;
    current.con->setCloseCallback(
        [collocProxy, trace, logger](const shared_ptr<Ice::Connection>&)
        {
            try
            {
                collocProxy->destroy();
                if (trace)
                {
                    Ice::Trace out(logger, "info");
                    out << "Session: " << collocProxy << " reaped.";
                }
            }
            catch (const Ice::LocalException&)
            {
                // Session already destroyed or server shutting down
            }
        });
    if (_trace)
    {
        Ice::Trace out(_logger, "info");
        out << "Poll session created for user '" << vname << "'.";
    }
    return proxy;
}
