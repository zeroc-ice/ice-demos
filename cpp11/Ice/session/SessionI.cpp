// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <SessionI.h>

using namespace std;
using namespace Demo;

class HelloI : public Hello
{
public:

    HelloI(const string& name, int id) :
        _name(name),
        _id(id)
    {
    }

    virtual ~HelloI()
    {
        cout << "Hello object #" << _id << " for session `" << _name << "' destroyed" << endl;
    }

    void
    sayHello(const Ice::Current&) override
    {
        cout << "Hello object #" << _id << " for session `" << _name << "' says:\n"
             << "Hello " << _name << "!" << endl;
    }

private:

    const string _name;
    const int _id;
};

SessionI::SessionI(const string& name) :
    _name(name),
    _nextId(0),
    _destroy(false)
{
    cout << "The session " << _name << " is now created." << endl;
}

shared_ptr<HelloPrx>
SessionI::createHello(const Ice::Current& current)
{
    lock_guard<mutex> sync(_mutex);
    if(_destroy)
    {
        throw Ice::ObjectNotExistException(__FILE__, __LINE__);
    }

    auto hello = Ice::uncheckedCast<HelloPrx>(current.adapter->addWithUUID(
                                                  make_shared<HelloI>(_name, _nextId++)));
    _objs.push_back(hello);
    return hello;
}

string
SessionI::getName(const Ice::Current&)
{
    lock_guard<mutex> sync(_mutex);
    if(_destroy)
    {
        throw Ice::ObjectNotExistException(__FILE__, __LINE__);
    }

    return _name;
}

void
SessionI::destroy(const Ice::Current& current)
{
    lock_guard<mutex> sync(_mutex);
    if(_destroy)
    {
        throw Ice::ObjectNotExistException(__FILE__, __LINE__);
    }

    _destroy = true;

    cout << "The session " << _name << " is now destroyed." << endl;
    try
    {
        current.adapter->remove(current.id);
        for(const auto& p: _objs)
        {
            current.adapter->remove(p->ice_getIdentity());
        }
    }
    catch(const Ice::ObjectAdapterDeactivatedException&)
    {
        // This method is called on shutdown of the server, in which
        // case this exception is expected.
    }

    _objs.clear();
}
