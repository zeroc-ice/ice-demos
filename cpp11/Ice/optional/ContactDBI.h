// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef CONTACT_I_H
#define CONTACT_I_H

#include <Contact.h>

class ContactDBI : public Demo::ContactDB
{
public:

    virtual void addContact(std::string, Ice::optional<Demo::NumberType>, Ice::optional<std::string>, Ice::optional<int>,
                            const Ice::Current&) override;

    virtual void updateContact(std::string, Ice::optional<Demo::NumberType>, Ice::optional<std::string>, Ice::optional<int>,
                               const Ice::Current&) override;

    virtual Demo::ContactPtr query(std::string, const Ice::Current&) override;

    virtual Ice::optional<std::string> queryNumber(std::string, const Ice::Current&) override;

    virtual void queryDialgroup(std::string, Ice::optional<int>&, const Ice::Current&) override;

    virtual void shutdown(const Ice::Current&) override;

private:

    std::map<std::string, std::shared_ptr<Demo::Contact>> _contacts;
};

#endif
