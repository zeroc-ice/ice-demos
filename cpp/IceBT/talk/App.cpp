// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <IceUtil/UUID.h>
#include <IceBT/ConnectionInfo.h>
#include <Talk.h>

using namespace std;

class PeerI;
typedef IceUtil::Handle<PeerI> PeerIPtr;

//
// We use two implementations of Peer servants. PeerI is the common
// base class for our servants.
//
// The program only supports one active connection at a time. That
// connection can either be initiated by a peer (an "incoming"
// connection) or it can be initiated by this program (an "outgoing"
// connection. We use different servants for each type of connection.
//
class PeerI : public Talk::Peer
{
public:

    virtual void sendMessage(const string& message) = 0;

    //
    // Disconnect from the remote peer.
    //
    virtual void close()
    {
        Talk::PeerPrx peer;

        {
            IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_lock);

            if(!_connected)
            {
                cout << ">>>> Not connected!" << endl;
                return;
            }

            _connected = false;
            peer = _peer;
            _peer = 0;
        }

        //
        // Try to notify the remote peer that we are disconnecting.
        //
        try
        {
            peer->disconnect();
            Ice::ConnectionPtr con = peer->ice_getCachedConnection();
            if(con)
            {
                con->close(false);
            }
        }
        catch(const Ice::Exception& ex)
        {
            cout << ">>>> Error: " << ex << endl;
        }
    }

    virtual void send(const string& message, const Ice::Current&)
    {
        IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_lock);

        cout << "Peer says: " << message << endl;
    }

    virtual void disconnect(const Ice::Current& curr)
    {
        IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_lock);

        _connected = false;

        cout << ">>>> Peer disconnected" << endl;

        curr.con->close(false);
    }

protected:

    PeerI() :
        _connected(false)
    {
    }

    PeerI(const Talk::PeerPrx& peer) :
        _connected(false),
        _peer(peer)
    {
    }

    //
    // We install this callback on the Bluetooth connection so that
    // we're notified when the connection closes.
    //
    class CloseCallbackI : public Ice::CloseCallback
    {
    public:

        CloseCallbackI(const PeerIPtr& p) :
            _obj(p)
        {
        }

        virtual void closed(const Ice::ConnectionPtr&)
        {
            _obj->closed();
        }

    private:

        PeerIPtr _obj;
    };
    friend class CloseCallbackI;

    virtual void closed()
    {
        IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_lock);

        if(_connected)
        {
            _connected = false;
            cout << ">>>> Lost connection to peer" << endl;
        }
    }

    IceUtil::Monitor<IceUtil::Mutex> _lock;
    bool _connected;
    Talk::PeerPrx _peer;
};

//
// This servant handles new incoming connections from peers.
// There is only one instance of this servant. It must be registered
// with a well-known object identity so that other peers can invoke
// on it.
//
class IncomingPeerI : public PeerI
{
public:

    //
    // Send a message to the remote peer.
    //
    virtual void sendMessage(const string& message)
    {
        {
            IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_lock);

            if(!_connected)
            {
                cout << ">>>> Not connected!" << endl;
                return;
            }
        }

        try
        {
            _peer->send(message);
        }
        catch(const Ice::Exception& ex)
        {
            cout << ">>>> Error: " << ex << endl;
        }
    }

    //
    // A remote peer wants to connect to us.
    //
    virtual void connect(const Ice::Identity& id, const Ice::Current& curr)
    {
        IceUtil::Monitor<IceUtil::Mutex>::Lock lock(_lock);

        if(_connected)
        {
            throw Talk::ConnectionException("already connected");
        }

        _connected = true;

        //
        // Install a connection callback and enable ACM heartbeats.
        //
        curr.con->setCloseCallback(new CloseCallbackI(this));
        curr.con->setACM(30, Ice::CloseOff, Ice::HeartbeatAlways);

        _peer = Talk::PeerPrx::uncheckedCast(curr.con->createProxy(id));

        Ice::ConnectionInfoPtr info = curr.con->getInfo();
        if(info->underlying)
        {
            info = info->underlying;
        }
        IceBT::ConnectionInfoPtr btInfo = IceBT::ConnectionInfoPtr::dynamicCast(info);
        assert(btInfo);
        cout << ">>>> Incoming connection from " << btInfo->remoteAddress << endl;
    }
};

//
// We use a new instance of this servant class for each new outgoing
// connection. Its primary purpose is to receive callbacks over the
// bidirectional connection to the peer.
//
class OutgoingPeerI : public PeerI
{
public:

    OutgoingPeerI(const Ice::ObjectAdapterPtr& adapter, const Ice::Identity& id, const Talk::PeerPrx& peer) :
        PeerI(peer),
        _adapter(adapter),
        _id(id)
    {
        _connected = true;
    }

    //
    // Send a message to the remote peer.
    //
    virtual void sendMessage(const string& message)
    {
        _peer->send(message);
    }

    virtual void connect(const Ice::Identity&, const Ice::Current&)
    {
        //
        // We established an outgoing connection to the peer, so there's no need for
        // the peer to invoke connect() on this servant.
        //
        throw Talk::ConnectionException("already connected");
    }

protected:

    virtual void closed()
    {
        PeerI::closed();

        _adapter->remove(_id);
    }

private:

    Ice::ObjectAdapterPtr _adapter;
    Ice::Identity _id;
};

class TalkApp : public Ice::Application
{
public:

    TalkApp();
    virtual int run(int, char*[]);

private:

    void connect(const string&);
    void usage();

    Ice::ObjectAdapterPtr _adapter;
    PeerIPtr _incomingPeer;
    PeerIPtr _peer;
};

static string btUUID = "6a193943-1754-4869-8d0a-ddc5f9a2b294";
static string btsUUID = "043257a6-d67c-4000-aa62-2ffe4583d324";

int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
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
    _incomingPeer = new IncomingPeerI;
    _peer = _incomingPeer;
    _adapter->add(_peer, Ice::stringToIdentity("peer"));
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
                    connect(s);
                }
                else if(s == "/disconnect")
                {
                    _peer->close();
                    _peer = _incomingPeer;
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
                _peer->sendMessage(s);
            }
        }
    }
    while(cin.good());

    communicator()->destroy();
    return EXIT_SUCCESS;
}

void
TalkApp::connect(const string& cmd)
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
    PeerIPtr servant;

    try
    {
        string proxy = "peer:";
        if(secure)
        {
            proxy += "bts -a \"" + addr + "\" -u " + btsUUID;
        }
        else
        {
            proxy += "bt -a \"" + addr + "\" -u " + btUUID;
        }
        //
        // Create a proxy for the remote peer using the address given by the user
        // and the well-known UUID for the talk service.
        //
        Talk::PeerPrx prx = Talk::PeerPrx::uncheckedCast(communicator()->stringToProxy(proxy));
        cout << ">>>> Connecting to " << addr << endl;

        //
        // Configure an object adapter for the connection and add the servant. This enables
        // us to receive callbacks via this connection. Calling ice_getConnection() blocks
        // until the connection to the peer is established.
        //
        Ice::ConnectionPtr con = prx->ice_getConnection();
        con->setAdapter(_adapter);
        servant = new OutgoingPeerI(_adapter, id, prx);
        _adapter->add(servant, id);

        //
        // Now we're ready to notify the peer that we'd like to connect.
        //
        prx->connect(id);
        _peer = servant;
        cout << ">>>> Connected to " << addr << endl;
    }
    catch(const Ice::Exception& ex)
    {
        cout << ">>>> " << ex << endl;
        if(servant)
        {
            _adapter->remove(id);
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
    cout << "  /quit" << endl;
    cout << endl;
}
