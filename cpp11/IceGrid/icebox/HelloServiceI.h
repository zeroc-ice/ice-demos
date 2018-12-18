// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef HELLO_SERVICE_I_H
#define HELLO_SERVICE_I_H

#include <IceBox/IceBox.h>

class HelloServiceI : public IceBox::Service
{
public:

    virtual void start(const std::string&,
                       const std::shared_ptr<Ice::Communicator>&,
                       const Ice::StringSeq&) override;

    virtual void stop() override;

private:

    std::shared_ptr<Ice::ObjectAdapter> _adapter;
};

#endif
