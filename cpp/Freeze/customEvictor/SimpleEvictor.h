// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef SIMPLE_EVICTOR_H
#define SIMPLE_EVICTOR_H

#include <EvictorBase.h>
#include <CurrentDatabase.h>
#include <IceUtil/IceUtil.h>

class SimpleEvictor : public EvictorBase
{
public:

    SimpleEvictor(CurrentDatabase&, int);

protected:
    
    virtual Ice::ObjectPtr add(const Ice::Current&, Ice::LocalObjectPtr&);
    virtual void evict(const Ice::ObjectPtr&, const Ice::LocalObjectPtr&);

private:

    CurrentDatabase& _currentDb;
};

#endif
