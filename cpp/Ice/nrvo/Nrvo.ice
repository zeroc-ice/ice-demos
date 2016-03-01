// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

[["cpp:include:MyStringSeq.h"]]

module Demo
{

["cpp:type:MyStringSeq"] sequence<string> StringSeq;

interface Nrvo
{
    StringSeq op1();
    StringSeq op2();
    StringSeq op3(int size);
    void shutdown();
};

};

