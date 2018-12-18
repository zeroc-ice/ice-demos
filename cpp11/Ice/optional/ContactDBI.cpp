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
ContactDBI::addContact(string name, Ice::optional<NumberType> type, Ice::optional<string> number,
                       Ice::optional<int> dialGroup, const Ice::Current&)
{
    auto contact = make_shared<Contact>();
    contact->name = name;
    if(type)
    {
        contact->type = move(type);
    }
    if(number)
    {
        contact->number = move(number);
    }
    if(dialGroup)
    {
        contact->dialGroup = move(dialGroup);
    }
    auto p = _contacts.insert(make_pair(name, contact));
    if(!p.second)
    {
        p.first->second = contact;
    }
}

void
ContactDBI::updateContact(string name, Ice::optional<NumberType> type, Ice::optional<string> number,
                          Ice::optional<int> dialGroup, const Ice::Current&)
{
    auto p = _contacts.find(name);
    if(p != _contacts.end())
    {
        if(type)
        {
            p->second->type = move(type);
        }
        if(number)
        {
            p->second->number = move(number);
        }
        if(dialGroup)
        {
            p->second->dialGroup = move(dialGroup);
        }
    }
}

shared_ptr<Contact>
ContactDBI::query(string name, const Ice::Current&)
{
    auto p = _contacts.find(name);
    if(p != _contacts.end())
    {
        return p->second;
    }
    return 0;
}

Ice::optional<string>
ContactDBI::queryNumber(string name, const Ice::Current&)
{
    auto p = _contacts.find(name);
    if(p != _contacts.end())
    {
        return p->second->number;
    }
    return Ice::nullopt;
}

void
ContactDBI::queryDialgroup(string name, Ice::optional<int>& dialGroup, const Ice::Current&)
{
    auto p = _contacts.find(name);
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
