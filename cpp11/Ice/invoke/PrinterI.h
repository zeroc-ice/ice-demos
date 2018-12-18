// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef PRINTER_I_H
#define PRINTER_I_H

#include <Printer.h>

class PrinterI : public Ice::Blobject
{
public:

    virtual bool ice_invoke(std::vector<Ice::Byte>, std::vector<Ice::Byte>&,
                            const Ice::Current&) override;
};

#endif
