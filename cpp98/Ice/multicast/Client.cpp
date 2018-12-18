// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>

#include <Hello.h>
#include <Discovery.h>

using namespace std;
using namespace Demo;

class DiscoverReplyI : public DiscoverReply, public IceUtil::Monitor<IceUtil::Mutex>
{
public:

    virtual void
    reply(const Ice::ObjectPrx& obj, const Ice::Current&)
    {
        Lock sync(*this);
        if(!_obj)
        {
            _obj = obj;
        }
        notify();
    }

    Ice::ObjectPrx
    waitReply(const IceUtil::Time& timeout)
    {
        Lock sync(*this);
        IceUtil::Time end = IceUtil::Time::now() + timeout;
        while(!_obj)
        {
            IceUtil::Time delay = end - IceUtil::Time::now();
            if(delay > IceUtil::Time::seconds(0))
            {
                timedWait(delay);
            }
            else
            {
                break;
            }
        }
        return _obj;
    }

private:

    Ice::ObjectPrx _obj;
};
typedef IceUtil::Handle<DiscoverReplyI> DiscoverReplyIPtr;

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
    Ice::registerIceUDP();
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
        Ice::CommunicatorHolder ich(argc, argv, "config.client");
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
            Ice::ObjectAdapterPtr adapter = communicator->createObjectAdapter("DiscoverReply");
            DiscoverReplyIPtr replyI = new DiscoverReplyI;
            DiscoverReplyPrx reply = DiscoverReplyPrx::uncheckedCast(adapter->addWithUUID(replyI));
            adapter->activate();

            DiscoverPrx discover = DiscoverPrx::uncheckedCast(
                communicator->propertyToProxy("Discover.Proxy")->ice_datagram());
            discover->lookup(reply);
            Ice::ObjectPrx base = replyI->waitReply(IceUtil::Time::seconds(2));
            if(!base)
            {
                cerr << argv[0] << ": no replies" << endl;
                return 1;
            }
            HelloPrx hello = HelloPrx::checkedCast(base);
            if(!hello)
            {
                cerr << argv[0] << ": invalid reply" << endl;
                return 1;
            }

            hello->sayHello();
        }
    }
    catch(const std::exception& ex)
    {
        cerr << ex.what() << endl;
        status = 1;
    }

    return status;
}
