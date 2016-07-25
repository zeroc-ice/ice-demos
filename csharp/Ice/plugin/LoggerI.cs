// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using System;

public class LoggerI : Ice.Logger
{
    public void print(string message)
    {
        Console.WriteLine("PRINT: " + message);
    }
    
    public void trace(string category, string message)
    {
        Console.WriteLine("TRACE(" + category + "): " + message);
    }
    
    public void warning(string message)
    {
        Console.WriteLine("WARNING: " + message);
    }
    
    public void error(string message)
    {
        Console.WriteLine("ERROR: " + message);
    }

    public string getPrefix()
    {
        return "";
    }

    public Ice.Logger cloneWithPrefix(string prefix)
    {
        return new LoggerI();
    }
}
