// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef SERVANT_FACTORY_H
#define SERVANT_FACTORY_H

#include <Ice/Ice.h>

class ValueFactory : public Ice::ValueFactory
{
public:

    virtual Ice::ObjectPtr create(const std::string&);
};

#endif
