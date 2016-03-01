// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

module Demo
{

interface CounterObserver
{
    void init(int value);
    void inc(int value);
};

interface Counter
{
    void subscribe(CounterObserver* observer);
    void unsubscribe(CounterObserver* observer);
    void inc(int value);
};

};

