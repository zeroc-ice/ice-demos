// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using System.Threading;
using ZeroC.Ice;

namespace Demo
{
    public class SessionFactory : ISessionFactory
    {
        public ISessionPrx Create(string name, Current current, CancellationToken cancel)
        {
            var session = new Session(name);
            ISessionPrx proxy = current.Adapter.AddWithUUID(session, ISessionPrx.Factory);

            // Remove endpoints to ensure that calls are collocated-only. This way, if we invoke on the proxy during
            // shutdown, the invocation fails immediately without attempting to establish any connection.
            ISessionPrx collocProxy = proxy.Clone(endpoints: Array.Empty<Endpoint>());

            // Never close this connection from the client and turn on heartbeats with a timeout of 30s
            // TODO:
            //          current.Connection!.IdleTimeout = TimeSpan.FromSeconds(30);
            // Removed for function, but this demo is somewhat pointless without this though
            current.Connection!.KeepAlive = true;
            current.Connection!.Closed += (sender, args) =>
            {
                try
                {
                    collocProxy.DestroyAsync();
                    Console.WriteLine("Cleaned up dead client.");
                }
                catch
                {
                    // The client already destroyed this session, or the server is shutting down
                }
            };
            return proxy;
        }

        public void Shutdown(Current current, CancellationToken cancel)
        {
            Console.Out.WriteLine("Shutting down...");
            current.Communicator.ShutdownAsync();
        }
    }
}
