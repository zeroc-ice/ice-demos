// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <IceUtil/UUID.h>
#include <IceBT/ConnectionInfo.h>
#include <IceBT/Plugin.h>
#include <Talk.h>
#include <mutex>

using namespace std;

class TalkApp : public Ice::Application
{
public:

    TalkApp();
    virtual int run(int, char*[]);

    void connect(Ice::Identity, shared_ptr<Ice::Connection>);
    void message(string);
    void disconnect(Ice::Identity, shared_ptr<Ice::Connection>, bool);
    void closed();

private:

    void doConnect(string);
    void doList();
    void doDisconnect();
    void doMessage(string);
    void failed(const Ice::LocalException&);
    void usage();

    shared_ptr<Ice::ObjectAdapter> _adapter;
    shared_ptr<Talk::PeerPrx> _local;
    shared_ptr<Talk::PeerPrx> _remote;
    mutex _mutex;
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

    virtual void connect(Ice::Identity id, const Ice::Current& current)
    {
        _app->connect(id, current.con);
    }

    virtual void send(string text, const Ice::Current&)
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

    virtual void connect(Ice::Identity, const Ice::Current&)
    {
        throw Talk::ConnectionException("already connected");
    }

    virtual void send(string text, const Ice::Current&)
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
    Ice::Application(Ice::SignalPolicy::NoSignalHandling)
{
}

int
TalkApp::run(int argc, char*[])
{
    if(argc > 1)
    {
        cerr << appName() << ": too many arguments" << endl;
        return EXIT_FAILURE;
    }

    //
    // Create an object adapter with the name "Talk". Its endpoint is defined
    // in the configuration file 'config'.
    //
    _adapter = communicator()->createObjectAdapter("Talk");

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

    return EXIT_SUCCESS;
}

void
TalkApp::connect(Ice::Identity id, shared_ptr<Ice::Connection> con)
{
    //
    // Called for a new incoming connection request.
    //

    unique_lock<mutex> lock(_mutex);

    if(_remote)
    {
        throw Talk::ConnectionException("already connected");
    }

    //
    // Install a connection callback and enable ACM heartbeats.
    //
    con->setCloseCallback(
        [this](shared_ptr<Ice::Connection>)
        {
            this->closed();
        });
    con->setACM(30, Ice::ACMClose::CloseOff, Ice::ACMHeartbeat::HeartbeatAlways);

    _remote = Ice::uncheckedCast<Talk::PeerPrx>(con->createProxy(id))->ice_invocationTimeout(10000);

    shared_ptr<Ice::ConnectionInfo> info = con->getInfo();
    if(info->underlying)
    {
        info = info->underlying;
    }
    shared_ptr<IceBT::ConnectionInfo> btInfo = dynamic_pointer_cast<IceBT::ConnectionInfo>(info);
    assert(btInfo);
    cout << ">>>> Incoming connection from " << btInfo->remoteAddress << endl;
}

void
TalkApp::message(string text)
{
    unique_lock<mutex> lock(_mutex);

    if(_remote)
    {
        cout << "Peer says: " << text << endl;
    }
}

void
TalkApp::disconnect(Ice::Identity id, shared_ptr<Ice::Connection> con, bool incoming)
{
    unique_lock<mutex> lock(_mutex);

    if(_remote)
    {
        cout << ">>>> Peer disconnected" << endl;
        _remote = 0;
    }

    if(!incoming)
    {
        _adapter->remove(id);
    }

    con->close(Ice::ConnectionClose::Gracefully);
}

void
TalkApp::closed()
{
    unique_lock<mutex> lock(_mutex);

    _remote = 0;

    cout << ">>>> Connection to peer closed" << endl;
}

void
TalkApp::doConnect(string cmd)
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

    //
    // Generate a UUID for our callback servant. We have to pass this identity to
    // the remote peer so that it can invoke callbacks on the servant over a
    // bidirectional connection.
    //
    Ice::Identity id = Ice::stringToIdentity(Ice::generateUUID());
    shared_ptr<Talk::PeerPrx> remote;

    try
    {
        {
            unique_lock<mutex> lock(_mutex);

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
            remote = Ice::uncheckedCast<Talk::PeerPrx>(communicator()->stringToProxy(proxy));
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
        _adapter->add(make_shared<OutgoingPeerI>(this), id);

        //
        // Install a connection callback and enable ACM heartbeats.
        //
        con->setCloseCallback(
            [this](shared_ptr<Ice::Connection>)
            {
                this->closed();
            });
        con->setACM(30, Ice::ACMClose::CloseOff, Ice::ACMHeartbeat::HeartbeatAlways);


        //
        // Now we're ready to notify the peer that we'd like to connect.
        //
        remote->connect(id);
        cout << ">>>> Connected to " << addr << endl;
    }
    catch(const Talk::ConnectionException& ex)
    {
        unique_lock<mutex> lock(_mutex);

        cout << ">>>> Connection failed: " << ex.reason << endl;
        try
        {
            _adapter->remove(id);
        }
        catch(const Ice::NotRegisteredException&)
        {
        }

        if(_remote == remote)
        {
            _remote = 0;
        }
    }
    catch(const Ice::Exception& ex)
    {
        unique_lock<mutex> lock(_mutex);

        cout << ">>>> " << ex << endl;
        try
        {
            _adapter->remove(id);
        }
        catch(const Ice::NotRegisteredException&)
        {
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
    auto plugin = dynamic_pointer_cast<IceBT::Plugin>(communicator()->getPluginManager()->getPlugin("IceBT"));
    assert(plugin);

    auto devices = plugin->getDevices();
    if(devices.empty())
    {
        cout << ">>>> No known devices" << endl;
    }
    else
    {
        for(auto p : devices)
        {
            string name;
            auto q = p.second.find("Name");
            if(q == p.second.end())
            {
                q = p.second.find("Alias");
            }
            if(q == p.second.end())
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
    shared_ptr<Talk::PeerPrx> peer;

    {
        unique_lock<mutex> lock(_mutex);

        if(!_remote)
        {
            cout << ">>>> Not connected" << endl;
            return;
        }

        peer = _remote;
        _remote = 0;
    }

    shared_ptr<Ice::Connection> con = peer->ice_getCachedConnection();

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
        con->close(Ice::ConnectionClose::Gracefully);
    }
}

void
TalkApp::doMessage(string text)
{
    shared_ptr<Talk::PeerPrx> peer;

    {
        unique_lock<mutex> lock(_mutex);

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
    shared_ptr<Talk::PeerPrx> peer;

    {
        unique_lock<mutex> lock(_mutex);
        peer = _remote;
        _remote = 0;
    }

    cout << ">>>> Action failed:" << endl << ex << endl;

    if(peer)
    {
        shared_ptr<Ice::Connection> con = peer->ice_getCachedConnection();
        if(con)
        {
            con->close(Ice::ConnectionClose::Forcefully);
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
