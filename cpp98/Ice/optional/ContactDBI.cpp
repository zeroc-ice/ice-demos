// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <ContactDBI.h>

using namespace std;
using namespace Demo;

void
ContactDBI::addContact(const string& name, const IceUtil::Optional<NumberType>& type,
    const IceUtil::Optional<string>& number, const IceUtil::Optional<Ice::Int>& dialGroup,
    const Ice::Current&)
{
    ContactPtr contact = new Contact();
    contact->name = name;
    if(type)
    {
        contact->type = type;
    }
    if(number)
    {
        contact->number = number;
    }
    if(dialGroup)
    {
        contact->dialGroup = dialGroup;
    }
    pair<map<string, ContactPtr>::iterator, bool> p = _contacts.insert(make_pair(name, contact));
    if(!p.second)
    {
        p.first->second = contact;
    }
}

void
ContactDBI::updateContact(const string& name, const IceUtil::Optional<NumberType>& type,
    const IceUtil::Optional<string>& number, const IceUtil::Optional<Ice::Int>& dialGroup,
    const Ice::Current&)
{
    map<string, ContactPtr>::iterator p = _contacts.find(name);
    if(p != _contacts.end())
    {
        if(type)
        {
            p->second->type = type;
        }
        if(number)
        {
            p->second->number = number;
        }
        if(dialGroup)
        {
            p->second->dialGroup = dialGroup;
        }
    }
}

ContactPtr
ContactDBI::query(const string& name, const Ice::Current&)
{
    map<string, ContactPtr>::const_iterator p = _contacts.find(name);
    if(p != _contacts.end())
    {
        return p->second;
    }
    return 0;
}

IceUtil::Optional<string>
ContactDBI::queryNumber(const string& name, const Ice::Current&)
{
    map<string, ContactPtr>::const_iterator p = _contacts.find(name);
    if(p != _contacts.end())
    {
        return p->second->number;
    }
    return IceUtil::None;
}

void
ContactDBI::queryDialgroup(const string& name, IceUtil::Optional<Ice::Int>& dialGroup, const Ice::Current&)
{
    map<string, ContactPtr>::const_iterator p = _contacts.find(name);
    if(p != _contacts.end())
    {
        dialGroup = p->second->dialGroup;
    }
}

void
ContactDBI::shutdown(const Ice::Current& c)
{
    cout << "Shutting down..." << endl;
    c.adapter->getCommunicator()->shutdown();
}
