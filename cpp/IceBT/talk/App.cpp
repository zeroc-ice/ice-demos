//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include "Talk.h"
#include <Ice/Ice.h>
#include <IceBT/IceBT.h>
#include <iostream>
#include <mutex>

using namespace std;

class TalkApp
{
public:
    int run(const shared_ptr<Ice::Communicator>& communicator);

    void connect(const optional<Talk::PeerPrx>&);
    void message(const string&);
    void disconnect(const Ice::Identity&, const shared_ptr<Ice::Connection>&, bool);
    void closed();

private:
    void doConnect(const string&);
    void doList();
    void doDisconnect();
    void doMessage(const string&);
    void failed(const Ice::LocalException&);
    void usage();

    shared_ptr<Ice::Communicator> _communicator;
    shared_ptr<Ice::ObjectAdapter> _adapter;
    optional<Talk::PeerPrx> _local;
    optional<Talk::PeerPrx> _remote;
    mutex _mutex;
};

//
// This servant listens for incoming connections from peers.
//
class IncomingPeerI final : public Talk::Peer
{
public:
    IncomingPeerI(TalkApp* app) : _app(app) {}

    void connect(optional<Talk::PeerPrx> peer, const Ice::Current& current) final
    {
        _app->connect(peer->ice_fixed(current.con));
    }

    void send(string text, const Ice::Current&) final { _app->message(text); }

    void disconnect(const Ice::Current& current) final { _app->disconnect(current.id, current.con, true); }

private:
    TalkApp* _app;
};

//
// This servant handles an outgoing session with a peer.
//
class OutgoingPeerI final : public Talk::Peer
{
public:
    OutgoingPeerI(TalkApp* app) : _app(app) {}

    void connect(optional<Talk::PeerPrx>, const Ice::Current&) final
    {
        throw Talk::ConnectionException("already connected");
    }

    void send(string text, const Ice::Current&) final { _app->message(text); }

    void disconnect(const Ice::Current& current) final { _app->disconnect(current.id, current.con, false); }

private:
    TalkApp* _app;
};

static const char* btUUID = "6a193943-1754-4869-8d0a-ddc5f9a2b294";
static const char* btsUUID = "043257a6-d67c-4000-aa62-2ffe4583d324";

int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceBT();
#endif

    int status = 0;

    try
    {
        //
        // CommunicatorHolder's ctor initializes an Ice communicator,
        // and its dtor destroys this communicator.
        //
        const Ice::CommunicatorHolder ich(argc, argv, "config");
        const auto& communicator = ich.communicator();

        //
        // The communicator initialization removes all Ice-related arguments from argc/argv
        //
        if (argc > 1)
        {
            cerr << argv[0] << ": too many arguments" << endl;
            status = 1;
        }
        else
        {
            TalkApp app;
            status = app.run(communicator);
        }
    }
    catch (const std::exception& ex)
    {
        cerr << ex.what() << endl;
        status = 1;
    }

    return status;
}

int
TalkApp::run(const shared_ptr<Ice::Communicator>& communicator)
{
    _communicator = communicator;

    //
    // Create an object adapter with the name "Talk". Its endpoint is defined
    // in the configuration file 'config'.
    //
    _adapter = _communicator->createObjectAdapter("Talk");

    //
    // Install a servant with the well-known identity "peer".
    //
    _local = Ice::uncheckedCast<Talk::PeerPrx>(
        _adapter->add(make_shared<IncomingPeerI>(this), Ice::stringToIdentity("peer")));
    _adapter->activate();

    usage();

    cout << ">>>> Ready." << endl;

    do
    {
        string s;
        cout << "";
        getline(cin, s);

        if (!s.empty())
        {
            if (s[0] == '/')
            {
                if (s.size() > 8 && (s.substr(0, 8) == "/connect" || s.substr(0, 9) == "/sconnect"))
                {
                    doConnect(s);
                }
                else if (s == "/disconnect")
                {
                    doDisconnect();
                }
                else if (s == "/list")
                {
                    doList();
                }
                else if (s == "/quit")
                {
                    break;
                }
                else
                {
                    usage();
                }
            }
            else
            {
                doMessage(s);
            }
        }
    } while (cin.good());

    //
    // There may still be objects (connections and servants) that hold pointers to this object, so we destroy
    // the communicator here to make sure they get cleaned up first.
    //
    _communicator->destroy();

    return 0;
}

void
TalkApp::connect(const optional<Talk::PeerPrx>& peer)
{
    //
    // Called for a new incoming connection request.
    //

    lock_guard<mutex> lock(_mutex);

    if (_remote)
    {
        throw Talk::ConnectionException("already connected");
    }

    //
    // Install a connection callback and enable ACM heartbeats.
    //
    auto con = peer->ice_getConnection();
    con->setCloseCallback([this](const shared_ptr<Ice::Connection>&) { this->closed(); });

    _remote = peer->ice_invocationTimeout(10000);

    auto info = con->getInfo();
    if (info->underlying)
    {
        info = info->underlying;
    }
    auto btInfo = dynamic_pointer_cast<IceBT::ConnectionInfo>(info);
    assert(btInfo);
    cout << ">>>> Incoming connection from " << btInfo->remoteAddress << endl;
}

void
TalkApp::message(const string& text)
{
    lock_guard<mutex> lock(_mutex);

    if (_remote)
    {
        cout << "Peer says: " << text << endl;
    }
}

void
TalkApp::disconnect(const Ice::Identity& id, const shared_ptr<Ice::Connection>& con, bool incoming)
{
    lock_guard<mutex> lock(_mutex);

    if (_remote)
    {
        cout << ">>>> Peer disconnected" << endl;
        _remote = nullopt;
    }

    if (!incoming)
    {
        _adapter->remove(id);
    }

    con->close().get();
}

void
TalkApp::closed()
{
    lock_guard<mutex> lock(_mutex);

    _remote = nullopt;

    cout << ">>>> Connection to peer closed" << endl;
}

void
TalkApp::doConnect(const string& cmd)
{
    const bool secure = cmd.find("/sconnect") == 0;

    string::size_type sp = cmd.find(' ');
    if (sp == string::npos)
    {
        usage();
        return;
    }
    sp = cmd.find_first_not_of(' ', sp);
    if (sp == string::npos)
    {
        usage();
        return;
    }
    string addr = cmd.substr(sp);

    optional<Talk::PeerPrx> remote;
    optional<Talk::PeerPrx> local;
    try
    {
        {
            lock_guard<mutex> lock(_mutex);

            if (_remote)
            {
                cout << ">>>> Already connected" << endl;
                return;
            }

            //
            // Create a proxy for the remote peer using the address given by the user
            // and the well-known UUID for the talk service.
            //
            string proxy = "peer:";
            if (secure)
            {
                proxy += "bts -a \"" + addr + "\" -u " + string{btsUUID};
            }
            else
            {
                proxy += "bt -a \"" + addr + "\" -u " + string{btUUID};
            }
            remote = Ice::uncheckedCast<Talk::PeerPrx>(_communicator->stringToProxy(proxy));
            _remote = remote;
        }

        cout << ">>>> Connecting to " << addr << endl;

        //
        // Configure an object adapter for the connection and add the servant. This enables
        // us to receive callbacks via this connection. Calling ice_getConnection() blocks
        // until the connection to the peer is established.
        //
        auto con = remote->ice_getConnection();
        con->setAdapter(_adapter);
        local = Ice::uncheckedCast<Talk::PeerPrx>(_adapter->addWithUUID(make_shared<OutgoingPeerI>(this)));

        //
        // Install a connection callback and enable ACM heartbeats.
        //
        con->setCloseCallback([this](const shared_ptr<Ice::Connection>&) { this->closed(); });

        //
        // Now we're ready to notify the peer that we'd like to connect.
        //
        remote->connect(local);
        cout << ">>>> Connected to " << addr << endl;
    }
    catch (const Talk::ConnectionException& ex)
    {
        lock_guard<mutex> lock(_mutex);

        cout << ">>>> Connection failed: " << ex.reason << endl;
        if (local)
        {
            _adapter->remove(local->ice_getIdentity());
        }

        if (_remote == remote)
        {
            _remote = nullopt;
        }
    }
    catch (const Ice::Exception& ex)
    {
        lock_guard<mutex> lock(_mutex);

        cout << ">>>> " << ex << endl;
        if (local)
        {
            _adapter->remove(local->ice_getIdentity());
        }

        if (_remote == remote)
        {
            _remote = nullopt;
        }
    }
}

void
TalkApp::doList()
{
    //
    // Obtain a reference to the IceBT plug-in.
    //
    auto plugin = dynamic_pointer_cast<IceBT::Plugin>(_communicator->getPluginManager()->getPlugin("IceBT"));
    assert(plugin);

    auto devices = plugin->getDevices();
    if (devices.empty())
    {
        cout << ">>>> No known devices" << endl;
    }
    else
    {
        for (auto p : devices)
        {
            string name;
            auto q = p.second.find("Name");
            if (q == p.second.end())
            {
                q = p.second.find("Alias");
            }
            if (q == p.second.end())
            {
                name = "Unknown";
            }
            else
            {
                name = q->second;
            }
            cout << ">>>> " << name << " - " << p.first << endl;
        }
    }
}

void
TalkApp::doDisconnect()
{
    optional<Talk::PeerPrx> peer;

    {
        lock_guard<mutex> lock(_mutex);

        if (!_remote)
        {
            cout << ">>>> Not connected" << endl;
            return;
        }

        peer = _remote;
        _remote = nullopt;
    }

    auto con = peer->ice_getCachedConnection();

    try
    {
        peer->disconnect();
    }
    catch (const Ice::LocalException& ex)
    {
        failed(ex);
    }

    if (con)
    {
        con->close().get();
    }
}

void
TalkApp::doMessage(const string& text)
{
    optional<Talk::PeerPrx> peer;

    {
        lock_guard<mutex> lock(_mutex);

        if (!_remote)
        {
            cout << ">>>> Not connected" << endl;
            return;
        }

        peer = _remote;
    }

    try
    {
        peer->send(text);
    }
    catch (const Ice::LocalException& ex)
    {
        failed(ex);
    }
}

void
TalkApp::failed(const Ice::LocalException& ex)
{
    optional<Talk::PeerPrx> peer;

    {
        lock_guard<mutex> lock(_mutex);
        peer = _remote;
        _remote = nullopt;
    }

    cout << ">>>> Action failed:" << endl << ex << endl;

    if (peer)
    {
        auto con = peer->ice_getCachedConnection();
        if (con)
        {
            con->abort();
        }
    }
}

void
TalkApp::usage()
{
    cout << endl;
    cout << "Usage:" << endl;
    cout << "  /connect <addr>" << endl;
    cout << "  /sconnect <addr>" << endl;
    cout << "  /disconnect" << endl;
    cout << "  /list" << endl;
    cout << "  /quit" << endl;
    cout << endl;
}
