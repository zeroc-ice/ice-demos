// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <MyStringSeq.h>

#include <iostream>

using namespace std;

MyStringSeq::MyStringSeq()
{
}

MyStringSeq::MyStringSeq(size_t n) : 
    vector<string>(n)
{
}

MyStringSeq::MyStringSeq(size_t n, const string& str) : 
    vector<string>(n, str)
{
}

MyStringSeq::MyStringSeq(const MyStringSeq& seq) : 
    vector<string>(seq)
{
    cout << "MyStringSeq copy ctor" << endl;
}
