// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

public class HelloPluginFactoryI implements Ice.PluginFactory
{
     @Override
    public Ice.Plugin
     create(Ice.Communicator communicator, String name, String[] args)
     {
         return new HelloPluginI(communicator);
     }
}
