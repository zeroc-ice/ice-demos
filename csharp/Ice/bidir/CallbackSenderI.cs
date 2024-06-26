// Copyright (c) ZeroC, Inc.

using Demo;

internal class CallbackSenderI(Ice.Communicator communicator) : CallbackSenderDisp_
{
    public void Destroy()
    {
        lock (this)
        {
            Console.Out.WriteLine("destroying callback sender");
            _destroy = true;
            System.Threading.Monitor.Pulse(this);
        }
    }

    public override void addClient(CallbackReceiverPrx client, Ice.Current current)
    {
        lock (this)
        {
            Console.Out.WriteLine("adding client `" + Ice.Util.identityToString(client.ice_getIdentity()) + "'");
            _clients.Add((CallbackReceiverPrx)client.ice_fixed(current.con));
        }
    }

    public void Run()
    {
        int num = 0;
        lock (this)
        {
            while (!_destroy)
            {
                System.Threading.Monitor.Wait(this, 2000);

                if (!_destroy && _clients.Count > 0)
                {
                    ++num;

                    // Invoke callback on all clients; it's safe to do it with _mutex locked
                    // because Ice guarantees these async invocations never block the calling thread.
                    //
                    // The exception callback, if called, is called by an Ice thread, and never the
                    // calling thread.
                    foreach (var c in _clients)
                    {
                        c.callbackAsync(num).ContinueWith(t =>
                        {
                            if (t.Exception != null)
                            {
                                RemoveClient(c, t.Exception.InnerException);
                            }
                        });
                    }
                }
            }
        }
    }

    private void RemoveClient(CallbackReceiverPrx c, Exception ex)
    {
        lock (this)
        {
            if (_clients.Remove(c))
            {
                Console.Error.WriteLine("removing client `" + Ice.Util.identityToString(c.ice_getIdentity()) + "':\n" +
                                        ex);
            }
        }
    }

    private Ice.Communicator _communicator = communicator;
    private bool _destroy = false;
    private List<CallbackReceiverPrx> _clients = new List<CallbackReceiverPrx>();
}
