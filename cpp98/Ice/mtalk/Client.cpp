// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <MTalk.h>

using namespace std;

class PeerI;
typedef IceUtil::Handle<PeerI> PeerIPtr;

class ChatApp
{
public:

    int run();

    void discoveredPeer(const string&, const MTalk::PeerPrx&);
    void connect(const string&, const MTalk::PeerPrx&);
    void message(const string&);
    void disconnect(const Ice::Identity&, const Ice::ConnectionPtr&, bool);
    void closed();

private:

    void doConnect(const string&);
    void doList();
    void doDisconnect();
    void doMessage(const string&);
    void failed(const Ice::LocalException&);
    void usage();

    string _name;
    Ice::ObjectAdapterPtr _multicastAdapter;
    Ice::ObjectAdapterPtr _peerAdapter;
    MTalk::PeerPrx _local;
    MTalk::PeerPrx _remote;
    map<string, MTalk::PeerPrx> _peers;
    IceUtil::Monitor<IceUtil::Mutex> _lock;
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

    virtual void announce(const string& name, const MTalk::PeerPrx& peer, const Ice::Current&)
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

    virtual void connect(const string& name, const MTalk::PeerPrx& peer, const Ice::Current& current)
    {
        _app->connect(name, peer->ice_fixed(current.con));
    }

    virtual void message(const string& text, const Ice::Current&)
    {
        _app->message(text);
    }

    virtual void disconnect(const Ice::Current& current)
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

    virtual void connect(const string&, const MTalk::PeerPrx&, const Ice::Current&)
    {
        throw MTalk::ConnectionException("already connected");
    }

    virtual void message(const string& text, const Ice::Current&)
    {
        _app->message(text);
    }

    virtual void disconnect(const Ice::Current& current)
    {
        _app->disconnect(current.id, current.con, false);
    }

private:

    ChatApp* _app;
};

//
// Called when a connection is closed.
//
class CloseCallbackI : public Ice::CloseCallback
{
public:

    CloseCallbackI(ChatApp* app) :
        _app(app)
    {
    }

    virtual void closed(const Ice::ConnectionPtr&)
    {
        _app->closed();
    }

private:

    ChatApp* _app;
};

//
// This thread periodically broadcasts a discover message.
//
class DiscoverThread : public IceUtil::Thread, public IceUtil::Monitor<IceUtil::Mutex>
{
public:

    DiscoverThread(const MTalk::DiscoveryPrx& d, const string& n, const MTalk::PeerPrx& p) :
        IceUtil::Thread(n), _discovery(d), _proxy(p), _destroy(false)
    {
    }

    void destroy()
    {
        {
            IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*this);
            _destroy = true;
            notify();
        }

        getThreadControl().join();
    }

    virtual void run()
    {
        while(true)
        {
            {
                IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*this);
                timedWait(IceUtil::Time::seconds(2));

                if(_destroy)
                {
                    break;
                }

            }

            _discovery->announce(_name, _proxy);
        }
    }

private:

    MTalk::DiscoveryPrx _discovery;
    MTalk::PeerPrx _proxy;
    bool _destroy;
};
typedef IceUtil::Handle<DiscoverThread> DiscoverThreadPtr;

//
// Global variable for destroyCommunicator
//
Ice::CommunicatorPtr communicator;

//
// Callback for CtrlCHandler
//
void
destroyCommunicator(int)
{
    communicator->destroy();
}

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
        // CtrlCHandler must be created before the communicator or any other threads are started
        //
        Ice::CtrlCHandler ctrlCHandler;

        //
        // CommunicatorHolder's ctor initializes an Ice communicator,
        // and it's dtor destroys this communicator.
        //
        Ice::CommunicatorHolder ich(argc, argv, "config");
        communicator = ich.communicator();

        //
        // Destroy communicator on Ctrl-C
        //
        ctrlCHandler.setCallback(&destroyCommunicator);

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
            status = app.run();
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
ChatApp::run()
{
    //
    // Create two object adapters. Their endpoints are defined in the configuration file 'config'.
    //
    _multicastAdapter = communicator->createObjectAdapter("Multicast");
    _peerAdapter = communicator->createObjectAdapter("Peer");

    //
    // Install a servant with the well-known identity "discover". This servant receives multicast messages.
    //
    _multicastAdapter->add(new DiscoveryI(this), Ice::stringToIdentity("discover"));
    _multicastAdapter->activate();

    //
    // Install a servant with the well-known identity "peer".
    //
    _local = MTalk::PeerPrx::uncheckedCast(
        _peerAdapter->add(new IncomingPeerI(this), Ice::stringToIdentity("peer")));
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
    MTalk::DiscoveryPrx discovery =
        MTalk::DiscoveryPrx::uncheckedCast(
            communicator->propertyToProxy("Discovery.Proxy")->ice_datagram()->ice_collocationOptimized(false));

    DiscoverThreadPtr thread = new DiscoverThread(discovery, _name, _local);
    thread->start();

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

    thread->destroy();

    //
    // There may still be objects (connections and servants) that hold pointers to this object, so we destroy
    // the communicator here to make sure they get cleaned up first.
    //
    communicator->destroy();

    return 0;
}

void
ChatApp::discoveredPeer(const string& name, const MTalk::PeerPrx& peer)
{
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_lock);

    //
    // We also receive multicast messages that we send, so ignore requests from ourself.
    //
    if(name == _name)
    {
        return;
    }

    map<string, MTalk::PeerPrx>::iterator p = _peers.find(name);
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
ChatApp::connect(const string& name, const MTalk::PeerPrx& peer)
{
    //
    // Called for a new incoming connection request.
    //

    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_lock);

    if(_remote)
    {
        throw MTalk::ConnectionException("already connected");
    }

    //
    // Install a connection callback and enable ACM heartbeats.
    //
    Ice::ConnectionPtr con = peer->ice_getConnection();
    con->setCloseCallback(new CloseCallbackI(this));
    con->setACM(30, Ice::CloseOff, Ice::HeartbeatAlways);

    _remote = peer->ice_invocationTimeout(5000);

    Ice::ConnectionInfoPtr info = con->getInfo();
    if(info->underlying)
    {
        info = info->underlying;
    }
    Ice::IPConnectionInfoPtr ipInfo = Ice::IPConnectionInfoPtr::dynamicCast(info);
    assert(ipInfo);
    cout << ">>>> Incoming connection from " << name << " with address " << ipInfo->remoteAddress << endl;
}

void
ChatApp::message(const string& text)
{
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_lock);

    if(_remote)
    {
        cout << "Peer says: " << text << endl;
    }
}

void
ChatApp::disconnect(const Ice::Identity& id, const Ice::ConnectionPtr& con, bool incoming)
{
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_lock);

    if(_remote)
    {
        cout << ">>>> Peer disconnected" << endl;
        _remote = 0;
    }

    if(!incoming)
    {
        _peerAdapter->remove(id);
    }

    con->close(Ice::ConnectionCloseGracefully);
}

void
ChatApp::closed()
{
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_lock);

    _remote = 0;

    cout << ">>>> Connection to peer closed" << endl;
}

void
ChatApp::doConnect(const string& cmd)
{
    string::size_type sp = cmd.find(' ');
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
    string name = cmd.substr(sp);

    MTalk::PeerPrx remote;
    {
        IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_lock);

        if(_remote)
        {
            cout << ">>>> Already connected" << endl;
            return;
        }

        map<string, MTalk::PeerPrx>::iterator p = _peers.find(name);
        if(p == _peers.end())
        {
            cout << ">>>> No peer found matching `" << name << "'" << endl;
            return;
        }
        _remote = p->second;
        remote = _remote;
    }

    MTalk::PeerPrx localPeer;
    try
    {
        cout << ">>>> Connecting to " << name << endl;

        //
        // Configure an object adapter for the connection and add the servant. This enables
        // us to receive callbacks via this connection. Calling ice_getConnection() blocks
        // until the connection to the peer is established.
        //
        Ice::ConnectionPtr con = remote->ice_getConnection();
        con->setAdapter(_peerAdapter);

        //
        // Register our callback servant with a UUID identity. We pass the returned proxy
        // to the remote peer so that it can invoke callbacks on the servant over a
        // bidirectional connection.
        //
        localPeer = MTalk::PeerPrx::uncheckedCast(_peerAdapter->addWithUUID(new OutgoingPeerI(this)));

        //
        // Install a connection callback and enable ACM heartbeats.
        //
        con->setCloseCallback(new CloseCallbackI(this));
        con->setACM(30, Ice::CloseOff, Ice::HeartbeatAlways);

        //
        // Now we're ready to notify the peer that we'd like to connect.
        //
        remote->connect(_name, localPeer);
        cout << ">>>> Connected to " << name << endl;
    }
    catch(const MTalk::ConnectionException& ex)
    {
        IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_lock);

        cout << ">>>> Connection failed: " << ex.reason << endl;
        if(localPeer)
        {
            _peerAdapter->remove(localPeer->ice_getIdentity());
        }

        if(_remote == remote)
        {
            _remote = 0;
        }
    }
    catch(const Ice::Exception& ex)
    {
        IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_lock);

        cout << ">>>> " << ex << endl;
        if(localPeer)
        {
            _peerAdapter->remove(localPeer->ice_getIdentity());
        }

        if(_remote == remote)
        {
            _remote = 0;
        }
    }
}

void
ChatApp::doList()
{
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_lock);

    if(_peers.empty())
    {
        cout << ">>>> No known peers" << endl;
    }
    else
    {
        for(map<string, MTalk::PeerPrx>::iterator p = _peers.begin(); p != _peers.end(); ++p)
        {
            cout << ">>>> " << p->first << endl;
        }
    }
}

void
ChatApp::doDisconnect()
{
    MTalk::PeerPrx peer;

    {
        IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_lock);

        if(!_remote)
        {
            cout << ">>>> Not connected" << endl;
            return;
        }

        peer = _remote;
        _remote = 0;
    }

    try
    {
        peer->disconnect();
    }
    catch(const Ice::LocalException& ex)
    {
        failed(ex);
    }
}

void
ChatApp::doMessage(const string& text)
{
    MTalk::PeerPrx peer;

    {
        IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_lock);

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
    MTalk::PeerPrx peer;

    {
        IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_lock);
        peer = _remote;
        _remote = 0;
    }

    cout << ">>>> Action failed:" << endl << ex << endl;

    if(peer)
    {
        Ice::ConnectionPtr con = peer->ice_getCachedConnection();
        if(con)
        {
            con->close(Ice::ConnectionCloseForcefully);
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
