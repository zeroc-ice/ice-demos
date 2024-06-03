//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include "ContactDBI.h"
#include <Ice/Ice.h>
#include <iostream>

using namespace std;
using namespace Demo;

void
ContactDBI::addContact(
    string name,
    optional<NumberType> type,
    optional<string> number,
    optional<int> dialGroup,
    const Ice::Current&)
{
    auto contact = make_shared<Contact>();
    contact->name = name;
    if (type)
    {
        contact->type = std::move(type);
    }
    if (number)
    {
        contact->number = std::move(number);
    }
    if (dialGroup)
    {
        contact->dialGroup = std::move(dialGroup);
    }
    auto p = _contacts.insert(make_pair(name, contact));
    if (!p.second)
    {
        p.first->second = contact;
    }
}

void
ContactDBI::updateContact(
    string name,
    optional<NumberType> type,
    optional<string> number,
    optional<int> dialGroup,
    const Ice::Current&)
{
    auto p = _contacts.find(name);
    if (p != _contacts.end())
    {
        if (type)
        {
            p->second->type = type;
        }
        if (number)
        {
            p->second->number = number;
        }
        if (dialGroup)
        {
            p->second->dialGroup = dialGroup;
        }
    }
}

shared_ptr<Contact>
ContactDBI::query(string name, const Ice::Current&)
{
    auto p = _contacts.find(name);
    if (p != _contacts.end())
    {
        return p->second;
    }
    return nullptr;
}

optional<string>
ContactDBI::queryNumber(string name, const Ice::Current&)
{
    auto p = _contacts.find(name);
    if (p != _contacts.end())
    {
        return p->second->number;
    }
    return nullopt;
}

void
ContactDBI::queryDialgroup(string name, optional<int>& dialGroup, const Ice::Current&)
{
    auto p = _contacts.find(name);
    if (p != _contacts.end())
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
