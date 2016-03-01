// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <IceUtil/IceUtil.h>
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

    // Unregister from the Locator registry
    Ice::LocatorRegistryPrx registry = c.adapter->getCommunicator()->getDefaultLocator()->getRegistry();
    registry->setAdapterDirectProxy("Hello", 0);

    c.adapter->getCommunicator()->shutdown();
}
