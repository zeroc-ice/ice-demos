// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Demo;
using System;

public class HelloI : HelloDisp_
{
    public HelloI(string name, int id)
    {
        _name =  name;
        _id = id;
    }

    public override void sayHello(Ice.Current c) 
    {
        Console.Out.WriteLine("Hello object #" + _id + " for session `" + _name + "' says:\n" +
                              "Hello " + _name + "!");
    }

    private string _name;
    private int _id;
}
