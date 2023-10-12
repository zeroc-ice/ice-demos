//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#ifndef PRINTER_H
#define PRINTER_H

#include <Hello.h>

class Printer : public Demo::Hello
{
public:

    virtual void sayHello(const Ice::Current&);
};

#endif
