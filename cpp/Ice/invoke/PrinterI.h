//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#ifndef PRINTER_I_H
#define PRINTER_I_H

#include "Printer.h"

class PrinterI final : public Ice::Blobject
{
public:
    bool ice_invoke(std::vector<std::byte>, std::vector<std::byte>&, const Ice::Current&) final;
};

#endif
