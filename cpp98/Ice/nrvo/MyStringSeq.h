// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef MY_STRING_SEQ
#define MY_STRING_SEQ

#include <Ice/Ice.h>

#include <vector>
#include <string>

class MyStringSeq : public std::vector<std::string>
{
public:

    MyStringSeq();

    MyStringSeq(size_t);

    MyStringSeq(size_t, const std::string&);

    MyStringSeq(const MyStringSeq&);

};

#endif
