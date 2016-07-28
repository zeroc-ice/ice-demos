// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <CallbackI.h>

using namespace std;
using namespace Ice;
using namespace Demo;

CallbackSenderI::CallbackSenderI(shared_ptr<Communicator> communicator) :
    _communicator(move(communicator)),
    _destroy(false)
{
}

void
CallbackSenderI::destroy()
{
    {
        unique_lock<mutex> lock(_mutex);
        cout << "destroying callback sender" << endl;
        _destroy = true;
        _cv.notify_one();
    }

    _senderThread.join();
}

void
CallbackSenderI::addClient(Identity ident, const Current& current)
{
    unique_lock<mutex> lock(_mutex);
    cout << "adding client `" << Ice::identityToString(ident) << "'"<< endl;
    _clients.insert(Ice::uncheckedCast<CallbackReceiverPrx>(current.con->createProxy(ident)));
}

void
CallbackSenderI::start()
{
    thread t([this]()
        {
            int num = 0;
            bool destroyed = false;
            while(!destroyed)
            {
                set<shared_ptr<CallbackReceiverPrx>> clients;
                {
                    unique_lock<mutex> lock(this->_mutex);
                    this->_cv.wait_for(lock, chrono::seconds(2));

                    if(this->_destroy)
                    {
                        destroyed = true;
                        continue;
                    }

                    clients = this->_clients;
                }

                if(!clients.empty())
                {
                    ++num;
                    for(auto p : clients)
                    {
                        try
                        {
                            p->callback(num);
                        }
                        catch(const Exception& ex)
                        {
                            cerr << "removing client `" << Ice::identityToString(p->ice_getIdentity()) << "':\n"
                                 << ex << endl;

                            unique_lock<mutex> lock(_mutex);
                            this->_clients.erase(p);
                        }
                    }
                }
            }
        });
    _senderThread = move(t);
}
