// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <condition_variable>
#include <mutex>
#include <future>
#include <MTalk.h>

using namespace std;

class ChatApp
{
public:

    int run(const shared_ptr<Ice::Communicator>&);

    void discoveredPeer(const string&, const shared_ptr<MTalk::PeerPrx>&);
    void connect(const string&, const shared_ptr<MTalk::PeerPrx>&);
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

    string _name;
    shared_ptr<Ice::ObjectAdapter> _multicastAdapter;
    shared_ptr<Ice::ObjectAdapter> _peerAdapter;
    shared_ptr<MTalk::PeerPrx> _local;
    shared_ptr<MTalk::PeerPrx> _remote;
    map<string, shared_ptr<MTalk::PeerPrx>> _peers;
    mutex _mutex;
    condition_variable _condition;
};

//
// This servant listens for multicast messages.
//
class DiscoveryI : public MTalk::Discovery
{
public:

    DiscoveryI(ChatApp* app) :
        _app(app)
    {
    }

    virtual void announce(string name, shared_ptr<MTalk::PeerPrx> peer, const Ice::Current&) override
    {
        _app->discoveredPeer(name, peer);
    }

private:

    ChatApp* _app;
};

//
// This servant listens for incoming connections from peers.
//
class IncomingPeerI : public MTalk::Peer
{
public:

    IncomingPeerI(ChatApp* app) :
        _app(app)
    {
    }

    virtual void connect(string name, shared_ptr<MTalk::PeerPrx> peer, const Ice::Current& current) override
    {
        _app->connect(name, peer->ice_fixed(current.con));
    }

    virtual void message(string text, const Ice::Current&) override
    {
        _app->message(text);
    }

    virtual void disconnect(const Ice::Current& current) override
    {
        _app->disconnect(current.id, current.con, true);
    }

private:

    ChatApp* _app;
};

//
// This servant handles an outgoing session with a peer.
//
class OutgoingPeerI : public MTalk::Peer
{
public:

    OutgoingPeerI(ChatApp* app) :
        _app(app)
    {
    }

    virtual void connect(string, shared_ptr<MTalk::PeerPrx>, const Ice::Current&) override
    {
        throw MTalk::ConnectionException("already connected");
    }

    virtual void message(string text, const Ice::Current&) override
    {
        _app->message(text);
    }

    virtual void disconnect(const Ice::Current& current) override
    {
        _app->disconnect(current.id, current.con, false);
    }

private:

    ChatApp* _app;
};

//
// This task periodically broadcasts a discover message.
//
class DiscoverTask
{
public:

    DiscoverTask(shared_ptr<MTalk::DiscoveryPrx> d, string n, shared_ptr<MTalk::PeerPrx> p) :
        _discovery(d), _name(n), _proxy(p)
    {
    }

    ~DiscoverTask()
    {
        {
            lock_guard<mutex> lock(_mutex);
            _destroyed = true;
        }
        _condition.notify_one();

        if(_result.valid())
        {
            try
            {
                _result.get();
            }
            catch(const std::exception& ex)
            {
                cerr << "Discover task raised exception " << ex.what() << endl;
            }
        }
    }

    void start()
    {
        assert(!_result.valid());
        _result = async(launch::async,
                        [this]
                        {
                            while(true)
                            {
                                {
                                    unique_lock<mutex> lock(_mutex);
                                    _condition.wait_for(lock, chrono::seconds(2));

                                    if(_destroyed)
                                    {
                                        break;
                                    }
                                }

                                _discovery->announce(_name, _proxy);
                            }
                        });
    }

private:

    shared_ptr<MTalk::DiscoveryPrx> _discovery;
    string _name;
    shared_ptr<MTalk::PeerPrx> _proxy;
    bool _destroyed = false;
    mutex _mutex;
    condition_variable _condition;
    future<void> _result;
};

int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceSSL();
#endif

    int status = 0;

    try
    {
        //
        // CommunicatorHolder's ctor initializes an Ice communicator,
        // and its dtor destroys this communicator.
        //
        Ice::CommunicatorHolder ich(argc, argv, "config");
        auto communicator = ich.communicator();

        //
        // The communicator initialization removes all Ice-related arguments from argc/argv
        //
        if(argc > 1)
        {
            cerr << argv[0] << ": too many arguments" << endl;
            status = 1;
        }
        else
        {
            ChatApp app;
            status = app.run(communicator);
        }
    }
    catch(const std::exception& ex)
    {
        cerr << ex.what() << endl;
        status = 1;
    }

    return status;
}

int
ChatApp::run(const shared_ptr<Ice::Communicator>& communicator)
{
    //
    // Create two object adapters. Their endpoints are defined in the configuration file 'config'.
    //
    _multicastAdapter = communicator->createObjectAdapter("Multicast");
    _peerAdapter = communicator->createObjectAdapter("Peer");

    //
    // Install a servant with the well-known identity "discover". This servant receives multicast messages.
    //
    _multicastAdapter->add(make_shared<DiscoveryI>(this), Ice::stringToIdentity("discover"));
    _multicastAdapter->activate();

    //
    // Install a servant with the well-known identity "peer".
    //
    _local = Ice::uncheckedCast<MTalk::PeerPrx>(
        _peerAdapter->add(make_shared<IncomingPeerI>(this), Ice::stringToIdentity("peer")));
    _peerAdapter->activate();

    while(_name.empty())
    {
        cout << ">>>> Enter your name: " << flush;
        getline(cin, _name);
    }

    //
    // Construct a datagram proxy that we'll use for multicast.
    //
    // Note that we need to disable collocation optimization because we also create an object adapter with
    // the same endpoint and we always want our discovery announcements to be broadcast on the network.
    //
    auto discovery = Ice::uncheckedCast<MTalk::DiscoveryPrx>(
        communicator->propertyToProxy("Discovery.Proxy")->ice_datagram()->ice_collocationOptimized(false));

    {
        DiscoverTask task(discovery, _name, _local);
        task.start();

        usage();

        cout << ">>>> Ready." << endl;

        do
        {
            string s;
            cout << "";
            getline(cin, s);

            if(!s.empty())
            {
                if(s[0] == '/')
                {
                    if(s.size() > 8 && s.substr(0, 8) == "/connect")
                    {
                        doConnect(s);
                    }
                    else if(s == "/disconnect")
                    {
                        doDisconnect();
                    }
                    else if(s == "/list")
                    {
                        doList();
                    }
                    else if(s == "/quit")
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
        }
        while(cin.good());
    }

    //
    // There may still be objects (connections and servants) that hold pointers to this object, so we destroy
    // the communicator here to make sure they get cleaned up first.
    //
    communicator->destroy();

    return 0;
}

void
ChatApp::discoveredPeer(const string& name, const shared_ptr<MTalk::PeerPrx>& peer)
{
    lock_guard<mutex> lock(_mutex);

    //
    // We also receive multicast messages that we send, so ignore requests from ourself.
    //
    if(name == _name)
    {
        return;
    }

    auto p = _peers.find(name);
    if(p == _peers.end())
    {
        cout << endl << ">>>> Discovered peer " << name << endl;
        _peers[name] = peer;
    }
    else
    {
        p->second = peer;
    }
}

void
ChatApp::connect(const string& name, const shared_ptr<MTalk::PeerPrx>& peer)
{
    //
    // Called for a new incoming connection request.
    //

    lock_guard<mutex> lock(_mutex);

    if(_remote)
    {
        throw MTalk::ConnectionException("already connected");
    }

    //
    // Install a connection callback and enable ACM heartbeats.
    //
    auto con = peer->ice_getConnection();
    con->setCloseCallback(
        [this](const shared_ptr<Ice::Connection>&)
        {
            this->closed();
        });
    con->setACM(30, Ice::ACMClose::CloseOff, Ice::ACMHeartbeat::HeartbeatAlways);

    _remote = peer->ice_invocationTimeout(5000);

    auto info = con->getInfo();
    if(info->underlying)
    {
        info = info->underlying;
    }
    auto ipInfo = dynamic_pointer_cast<Ice::IPConnectionInfo>(info);
    assert(ipInfo);
    cout << ">>>> Incoming connection from " << name << " with address " << ipInfo->remoteAddress << endl;
}

void
ChatApp::message(const string& text)
{
    lock_guard<mutex> lock(_mutex);

    if(_remote)
    {
        cout << "Peer says: " << text << endl;
    }
}

void
ChatApp::disconnect(const Ice::Identity& id, const shared_ptr<Ice::Connection>& con, bool incoming)
{
    lock_guard<mutex> lock(_mutex);

    if(_remote)
    {
        cout << ">>>> Peer disconnected" << endl;
        _remote = nullptr;
    }

    if(!incoming)
    {
        _peerAdapter->remove(id);
    }

    con->close(Ice::ConnectionClose::Gracefully);
}

void
ChatApp::closed()
{
    lock_guard<mutex> lock(_mutex);

    _remote = nullptr;

    cout << ">>>> Connection to peer closed" << endl;
}

void
ChatApp::doConnect(const string& cmd)
{
    auto sp = cmd.find(' ');
    if(sp == string::npos)
    {
        usage();
        return;
    }
    sp = cmd.find_first_not_of(' ', sp);
    if(sp == string::npos)
    {
        usage();
        return;
    }
    auto name = cmd.substr(sp);

    shared_ptr<MTalk::PeerPrx> remote;
    {
        lock_guard<mutex> lock(_mutex);

        if(_remote)
        {
            cout << ">>>> Already connected" << endl;
            return;
        }

        auto p = _peers.find(name);
        if(p == _peers.end())
        {
            cout << ">>>> No peer found matching `" << name << "'" << endl;
            return;
        }
        _remote = p->second;
        remote = _remote;
    }

    shared_ptr<MTalk::PeerPrx> localPeer;
    try
    {
        cout << ">>>> Connecting to " << name << endl;

        //
        // Configure an object adapter for the connection and add the servant. This enables
        // us to receive callbacks via this connection. Calling ice_getConnection() blocks
        // until the connection to the peer is established.
        //
        auto con = remote->ice_getConnection();
        con->setAdapter(_peerAdapter);

        //
        // Register our callback servant with a UUID identity. We pass the returned proxy
        // to the remote peer so that it can invoke callbacks on the servant over a
        // bidirectional connection.
        //
        localPeer = Ice::uncheckedCast<MTalk::PeerPrx>(_peerAdapter->addWithUUID(make_shared<OutgoingPeerI>(this)));

        //
        // Install a connection callback and enable ACM heartbeats.
        //
        con->setCloseCallback(
            [this](const shared_ptr<Ice::Connection>&)
            {
                this->closed();
            });
        con->setACM(30, Ice::ACMClose::CloseOff, Ice::ACMHeartbeat::HeartbeatAlways);

        //
        // Now we're ready to notify the peer that we'd like to connect.
        //
        remote->connect(_name, localPeer);
        cout << ">>>> Connected to " << name << endl;
    }
    catch(const MTalk::ConnectionException& ex)
    {
        cout << ">>>> Connection failed: " << ex.reason << endl;
        if(localPeer)
        {
            _peerAdapter->remove(localPeer->ice_getIdentity());
        }
        _remote = nullptr;
        return;
    }
    catch(const Ice::Exception& ex)
    {
        cout << ">>>> " << ex << endl;
        if(localPeer)
        {
            _peerAdapter->remove(localPeer->ice_getIdentity());
        }
        _remote = nullptr;
        return;
    }
}

void
ChatApp::doList()
{
    lock_guard<mutex> lock(_mutex);

    if(_peers.empty())
    {
        cout << ">>>> No known peers" << endl;
    }
    else
    {
        for(const auto& p: _peers)
        {
            cout << ">>>> " << p.first << endl;
        }
    }
}

void
ChatApp::doDisconnect()
{
    shared_ptr<MTalk::PeerPrx> peer;

    {
        lock_guard<mutex> lock(_mutex);

        if(!_remote)
        {
            cout << ">>>> Not connected" << endl;
            return;
        }

        peer = _remote;
        _remote = nullptr;
    }

    try
    {
        peer->disconnect();
    }
    catch(const Ice::LocalException&)
    {
    }
}

void
ChatApp::doMessage(const string& text)
{
    shared_ptr<MTalk::PeerPrx> peer;

    {
        lock_guard<mutex> lock(_mutex);

        if(!_remote)
        {
            cout << ">>>> Not connected" << endl;
            return;
        }

        peer = _remote;
    }

    try
    {
        peer->message(text);
    }
    catch(const Ice::LocalException& ex)
    {
        failed(ex);
    }
}

void
ChatApp::failed(const Ice::LocalException& ex)
{
    shared_ptr<MTalk::PeerPrx> peer;

    {
        lock_guard<mutex> lock(_mutex);
        peer = _remote;
        _remote = nullptr;
    }

    cout << ">>>> Action failed:" << endl << ex << endl;

    if(peer)
    {
        auto con = peer->ice_getCachedConnection();
        if(con)
        {
            con->close(Ice::ConnectionClose::Forcefully);
        }
    }
}

void
ChatApp::usage()
{
    cout << endl;
    cout << "Usage:" << endl;
    cout << "  /connect <name>" << endl;
    cout << "  /disconnect" << endl;
    cout << "  /list" << endl;
    cout << "  /quit" << endl;
    cout << endl;
}
