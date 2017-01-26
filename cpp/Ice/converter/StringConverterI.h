// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef STRING_CONVERTER_I_H
#define STRING_CONVERTER_I_H

#include <Ice/Ice.h>

namespace Demo
{

//
// UTF-8 converter for LATIN-1
//
class StringConverterI : public Ice::StringConverter
{
public:

    StringConverterI();
    ~StringConverterI();

    virtual Ice::Byte* toUTF8(const char*, const char*, Ice::UTF8Buffer&) const;
    virtual void fromUTF8(const Ice::Byte*, const Ice::Byte*, std::string&) const;
};

}

#endif
