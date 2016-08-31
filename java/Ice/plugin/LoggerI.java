// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

public class LoggerI implements com.zeroc.Ice.Logger
{
    @Override
    public void print(String message)
    {
        System.out.println("PRINT: " + message);
    }
    
    @Override
    public void trace(String category, String message)
    {
        System.out.println("TRACE(" + category + "): " + message);
    }
    
    @Override
    public void warning(String message)
    {
        System.out.println("WARNING: " + message);
    }

    @Override
    public void error(String message)
    {
        System.out.println("ERROR: " + message);
    }

    @Override
    public String getPrefix()
    {
        return "";
    }

    @Override
    public com.zeroc.Ice.Logger cloneWithPrefix(String prefix)
    {
        return new LoggerI();
    }
}
