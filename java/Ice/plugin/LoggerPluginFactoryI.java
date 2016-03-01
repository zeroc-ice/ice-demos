// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************



public class LoggerPluginFactoryI implements Ice.PluginFactory
{
     @Override
    public Ice.Plugin
     create(Ice.Communicator communicator, String name, String[] args)
     {
         return new Ice.LoggerPlugin(communicator, new LoggerI());
     }
}
