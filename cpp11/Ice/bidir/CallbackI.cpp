// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <CallbackI.h>

using namespace std;
using namespace Ice;
using namespace Demo;

CallbackSenderI::CallbackSenderI(shared_ptr<Communicator> communicator) :
    _communicator(move(communicator))
{
}

void
CallbackSenderI::destroy()
{
    {
        lock_guard<mutex> lock(_mutex);
        cout << "destroying callback sender" << endl;
        _destroy = true;
    }
    _cv.notify_one();

    if(_result.valid())
    {
        // wait for async task to complete
        _result.get();
    }
}

void
CallbackSenderI::addClient(Identity ident, const Current& current)
{
    lock_guard<mutex> lock(_mutex);
    cout << "adding client `" << Ice::identityToString(ident) << "'"<< endl;
    _clients.insert(Ice::uncheckedCast<CallbackReceiverPrx>(current.con->createProxy(ident)));
}

void
CallbackSenderI::start()
{
    assert(!_result.valid()); // start should only be called once

    _result =
        async(launch::async,
              [this]
              {
                  int num = 0;
                  bool destroyed = false;
                  while(!destroyed)
                  {
                      set<shared_ptr<CallbackReceiverPrx>> clients;
                      {
                          unique_lock<mutex> lock(_mutex);
                          _cv.wait_for(lock, chrono::seconds(2));

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

                                  lock_guard<mutex> lock(_mutex);
                                  _clients.erase(p);
                              }
                          }
                      }
                  }
              });
}
