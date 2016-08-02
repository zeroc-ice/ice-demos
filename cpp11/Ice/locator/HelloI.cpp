// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <HelloI.h>

using namespace std;

void
HelloI::sayHello(const Ice::Current&)
{
    cout << "Hello World!" << endl;
}

void
HelloI::shutdown(const Ice::Current& c)
{
    cout << "Shutting down..." << endl;

    //
    // Unregister from the Locator registry
    //
    auto communicator = c.adapter->getCommunicator();
    communicator->getDefaultLocator()->getRegistry()->setAdapterDirectProxy("Hello", nullptr);
    communicator->shutdown();
}
