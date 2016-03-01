// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef HELLO_I_H
#define HELLO_I_H

#include <Hello.h>
#include <LogI.h>

class CDialog;

class HelloI : public Demo::Hello
{
public:

    HelloI(const LogIPtr&, CDialog*);

    virtual void sayHello(int, const Ice::Current&);
    virtual void shutdown(const Ice::Current&);

private:

    LogIPtr _log;
    CDialog* _dialog;
};

#endif
