// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

module Demo
{

interface Greet
{
    idempotent string exchangeGreeting(string s);
    void shutdown();
};

};
