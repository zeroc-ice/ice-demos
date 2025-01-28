// Copyright (c) ZeroC, Inc.

public class HelloPluginFactoryI implements com.zeroc.Ice.PluginFactory
{
    @Override
    public com.zeroc.Ice.Plugin create(com.zeroc.Ice.Communicator communicator, String name, String[] args)
    {
        return new HelloPluginI(communicator);
    }
}
