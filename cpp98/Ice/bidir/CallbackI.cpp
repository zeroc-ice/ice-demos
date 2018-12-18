// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <CallbackI.h>

using namespace std;
using namespace Ice;
using namespace Demo;

CallbackSenderI::CallbackSenderI() :
    _destroy(false)
{
}

void
CallbackSenderI::destroy()
{
    {
        IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*this);

        cout << "destroying callback sender" << endl;
        _destroy = true;

        notify();
    }

    getThreadControl().join();
}

void
CallbackSenderI::addClient(const CallbackReceiverPrx& client, const Current& current)
{
    IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*this);
    cout << "adding client `" << Ice::identityToString(client->ice_getIdentity()) << "'"<< endl;
    _clients.insert(client->ice_fixed(current.con));
}

void
CallbackSenderI::run()
{
    int num = 0;
    bool destroyed = false;
    while(!destroyed)
    {
        std::set<Demo::CallbackReceiverPrx> clients;
        {
            IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*this);
            timedWait(IceUtil::Time::seconds(2));

            if(_destroy)
            {
                destroyed = true;
                continue;
            }

            clients = _clients;
        }

        if(!clients.empty())
        {
            ++num;
            for(set<CallbackReceiverPrx>::iterator p = clients.begin(); p != clients.end(); ++p)
            {
                try
                {
                    (*p)->callback(num);
                }
                catch(const Exception& ex)
                {
                    cerr << "removing client `" << Ice::identityToString((*p)->ice_getIdentity()) << "':\n"
                         << ex << endl;

                    IceUtil::Monitor<IceUtil::Mutex>::Lock lck(*this);
                    _clients.erase(*p);
                }
            }
        }
    }
}
