//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#ifndef CONTACT_I_H
#define CONTACT_I_H

#include "Contact.h"

class ContactDBI final : public Demo::ContactDB
{
public:
    void addContact(
        std::string,
        std::optional<Demo::NumberType>,
        std::optional<std::string>,
        std::optional<int>,
        const Ice::Current&) final;

    void updateContact(
        std::string,
        std::optional<Demo::NumberType>,
        std::optional<std::string>,
        std::optional<int>,
        const Ice::Current&) final;

    Demo::ContactPtr query(std::string name, const Ice::Current& current) final;

    std::optional<std::string> queryNumber(std::string, const Ice::Current&) final;

    void queryDialgroup(std::string, std::optional<int>&, const Ice::Current&) final;

    void shutdown(const Ice::Current&) final;

private:
    std::map<std::string, std::shared_ptr<Demo::Contact>> _contacts;
};

#endif
