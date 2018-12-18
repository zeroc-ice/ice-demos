// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <IceBT/IceBT.h>
#include <Talk.h>

using namespace std;

class TalkApp : public Ice::Application
{
public:

    TalkApp();
    virtual int run(int, char*[]);

    void connect(const Talk::PeerPrx&);
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

    Ice::ObjectAdapterPtr _adapter;
    Talk::PeerPrx _local;
    Talk::PeerPrx _remote;
    IceUtil::Monitor<IceUtil::Mutex> _lock;
};

//
// This servant listens for incoming connections from peers.
//
class IncomingPeerI : public Talk::Peer
{
public:

    IncomingPeerI(TalkApp* app) :
        _app(app)
    {
    }

    virtual void connect(const Talk::PeerPrx& peer, const Ice::Current& current)
    {
        _app->connect(peer->ice_fixed(current.con));
    }

    virtual void send(const string& text, const Ice::Current&)
    {
        _app->message(text);
    }

    virtual void disconnect(const Ice::Current& current)
    {
        _app->disconnect(current.id, current.con, true);
    }

private:

    TalkApp* _app;
};

//
// This servant handles an outgoing session with a peer.
//
class OutgoingPeerI : public Talk::Peer
{
public:

    OutgoingPeerI(TalkApp* app) :
        _app(app)
    {
    }

    virtual void connect(const Talk::PeerPrx&, const Ice::Current&)
    {
        throw Talk::ConnectionException("already connected");
    }

    virtual void send(const string& text, const Ice::Current&)
    {
        _app->message(text);
    }

    virtual void disconnect(const Ice::Current& current)
    {
        _app->disconnect(current.id, current.con, false);
    }

private:

    TalkApp* _app;
};

//
// Called when a connection is closed.
//
class CloseCallbackI : public Ice::CloseCallback
{
public:

    CloseCallbackI(TalkApp* app) :
        _app(app)
    {
    }

    virtual void closed(const Ice::ConnectionPtr&)
    {
        _app->closed();
    }

private:

    TalkApp* _app;
};

static string btUUID = "6a193943-1754-4869-8d0a-ddc5f9a2b294";
static string btsUUID = "043257a6-d67c-4000-aa62-2ffe4583d324";

int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceSSL();
    Ice::registerIceBT();
#endif

    TalkApp app;
    return app.main(argc, argv, "config");
}

TalkApp::TalkApp() :
    //
    // Since this is an interactive demo we don't want any signal handling.
    //
    Ice::Application(Ice::NoSignalHandling)
{
}

int
TalkApp::run(int argc, char*[])
{
    if(argc > 1)
    {
        cerr << appName() << ": too many arguments" << endl;
        return 1;
    }

    //
    // Create an object adapter with the name "Talk". Its endpoint is defined
    // in the configuration file 'config'.
    //
    _adapter = communicator()->createObjectAdapter("Talk");

    //
    // Install a servant with the well-known identity "peer".
    //
    _local = Talk::PeerPrx::uncheckedCast(_adapter->add(new IncomingPeerI(this), Ice::stringToIdentity("peer")));
    _adapter->activate();

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
                if(s.size() > 8 && (s.substr(0, 8) == "/connect" || s.substr(0, 9) == "/sconnect"))
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

    //
    // There may still be objects (connections and servants) that hold pointers to this object, so we destroy
    // the communicator here to make sure they get cleaned up first.
    //
    communicator()->destroy();

    return 0;
}

void
TalkApp::connect(const Talk::PeerPrx& peer)
{
    //
    // Called for a new incoming connection request.
    //

    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_lock);

    if(_remote)
    {
        throw Talk::ConnectionException("already connected");
    }

    //
    // Install a connection callback and enable ACM heartbeats.
    //
    Ice::ConnectionPtr con = peer->ice_getConnection();
    con->setCloseCallback(new CloseCallbackI(this));
    con->setACM(30, Ice::CloseOff, Ice::HeartbeatAlways);

    _remote = peer->ice_invocationTimeout(10000);

    Ice::ConnectionInfoPtr info = con->getInfo();
    if(info->underlying)
    {
        info = info->underlying;
    }
    IceBT::ConnectionInfoPtr btInfo = IceBT::ConnectionInfoPtr::dynamicCast(info);
    assert(btInfo);
    cout << ">>>> Incoming connection from " << btInfo->remoteAddress << endl;
}

void
TalkApp::message(const string& text)
{
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_lock);

    if(_remote)
    {
        cout << "Peer says: " << text << endl;
    }
}

void
TalkApp::disconnect(const Ice::Identity& id, const Ice::ConnectionPtr& con, bool incoming)
{
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_lock);

    if(_remote)
    {
        cout << ">>>> Peer disconnected" << endl;
        _remote = 0;
    }

    if(!incoming)
    {
        _adapter->remove(id);
    }

    con->close(Ice::ConnectionCloseGracefully);
}

void
TalkApp::closed()
{
    IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_lock);

    _remote = 0;

    cout << ">>>> Connection to peer closed" << endl;
}

void
TalkApp::doConnect(const string& cmd)
{
    const bool secure = cmd.find("/sconnect") == 0;

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
    string addr = cmd.substr(sp);

    Talk::PeerPrx remote;
    Talk::PeerPrx local;
    try
    {
        {
            IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_lock);

            if(_remote)
            {
                cout << ">>>> Already connected" << endl;
                return;
            }

            //
            // Create a proxy for the remote peer using the address given by the user
            // and the well-known UUID for the talk service.
            //
            string proxy = "peer:";
            if(secure)
            {
                proxy += "bts -a \"" + addr + "\" -u " + btsUUID;
            }
            else
            {
                proxy += "bt -a \"" + addr + "\" -u " + btUUID;
            }
            remote = Talk::PeerPrx::uncheckedCast(communicator()->stringToProxy(proxy));
            _remote = remote;
        }

        cout << ">>>> Connecting to " << addr << endl;

        //
        // Configure an object adapter for the connection and add the servant. This enables
        // us to receive callbacks via this connection. Calling ice_getConnection() blocks
        // until the connection to the peer is established.
        //
        Ice::ConnectionPtr con = remote->ice_getConnection();
        con->setAdapter(_adapter);
        local = Talk::PeerPrx::uncheckedCast(_adapter->addWithUUID(new OutgoingPeerI(this)));

        //
        // Install a connection callback and enable ACM heartbeats.
        //
        con->setCloseCallback(new CloseCallbackI(this));
        con->setACM(30, Ice::CloseOff, Ice::HeartbeatAlways);

        //
        // Now we're ready to notify the peer that we'd like to connect.
        //
        remote->connect(local);
        cout << ">>>> Connected to " << addr << endl;
    }
    catch(const Talk::ConnectionException& ex)
    {
        IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_lock);

        cout << ">>>> Connection failed: " << ex.reason << endl;
        if(local)
        {
            _adapter->remove(local->ice_getIdentity());
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
        if(local)
        {
            _adapter->remove(local->ice_getIdentity());
        }

        if(_remote == remote)
        {
            _remote = 0;
        }
    }
}

void
TalkApp::doList()
{
    //
    // Obtain a reference to the IceBT plug-in.
    //
    IceBT::PluginPtr plugin = IceBT::PluginPtr::dynamicCast(communicator()->getPluginManager()->getPlugin("IceBT"));
    assert(plugin);

    IceBT::DeviceMap devices = plugin->getDevices();
    if(devices.empty())
    {
        cout << ">>>> No known devices" << endl;
    }
    else
    {
        for(IceBT::DeviceMap::iterator p = devices.begin(); p != devices.end(); ++p)
        {
            string name;
            IceBT::PropertyMap::iterator q = p->second.find("Name");
            if(q == p->second.end())
            {
                q = p->second.find("Alias");
            }
            if(q == p->second.end())
            {
                name = "Unknown";
            }
            else
            {
                name = q->second;
            }
            cout << ">>>> " << name << " - " << p->first << endl;
        }
    }
}

void
TalkApp::doDisconnect()
{
    Talk::PeerPrx peer;

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

    Ice::ConnectionPtr con = peer->ice_getCachedConnection();

    try
    {
        peer->disconnect();
    }
    catch(const Ice::LocalException& ex)
    {
        failed(ex);
    }

    if(con)
    {
        con->close(Ice::ConnectionCloseGracefully);
    }
}

void
TalkApp::doMessage(const string& text)
{
    Talk::PeerPrx peer;

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
        peer->send(text);
    }
    catch(const Ice::LocalException& ex)
    {
        failed(ex);
    }
}

void
TalkApp::failed(const Ice::LocalException& ex)
{
    Talk::PeerPrx peer;

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
