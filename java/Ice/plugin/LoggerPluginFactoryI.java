// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

public class LoggerPluginFactoryI implements com.zeroc.Ice.PluginFactory
{
    @Override
    public com.zeroc.Ice.Plugin create(com.zeroc.Ice.Communicator communicator, String name, String[] args)
    {
        return new com.zeroc.Ice.LoggerPlugin(communicator, new LoggerI());
    }
}
