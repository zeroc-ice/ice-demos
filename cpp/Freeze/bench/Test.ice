// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

module Demo
{

struct Struct1
{
    long l;
};

struct Struct2
{
    string s;
    Struct1 s1;
};

class Class1
{
    string s;
};

class Class2 extends Class1
{
    Object obj;
    Object rec;
};

};
