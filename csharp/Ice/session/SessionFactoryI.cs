// Copyright (c) ZeroC, Inc.

using Demo;

internal class SessionFactoryI : SessionFactoryDisp_
{
    public override SessionPrx create(string name, Ice.Current current)
    {
        var session = new SessionI(name);
        var proxy = SessionPrxHelper.uncheckedCast(current.adapter.addWithUUID(session));

        // Remove endpoints to ensure that calls are collocated-only
        // This way, if we invoke on the proxy during shutdown, the invocation fails immediately
        // without attempting to establish any connection
        var collocProxy = SessionPrxHelper.uncheckedCast(proxy.ice_endpoints(new Ice.Endpoint[0]));

        current.con.setCloseCallback(_ =>
            {
                try
                {
                    collocProxy.destroy();
                    Console.Out.WriteLine("Cleaned up dead client.");
                }
                catch (Ice.LocalException)
                {
                    // The client already destroyed this session, or the server is shutting down
                }
            });
        return proxy;
    }

    public override void shutdown(Ice.Current current)
    {
        Console.Out.WriteLine("Shutting down...");
        current.adapter.getCommunicator().shutdown();
    }
}
