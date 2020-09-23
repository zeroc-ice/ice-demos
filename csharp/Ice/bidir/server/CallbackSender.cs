// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using System.Collections.Generic;
using ZeroC.Ice;

namespace Demo
{
    class CallbackSender : ICallbackSender
    {
        private bool _destroy;
        private readonly List<ICallbackReceiverPrx> _clients;

        public CallbackSender()
        {
            _destroy = false;
            _clients = new List<ICallbackReceiverPrx>();
        }

        public void Destroy()
        {
            lock (this)
            {
                Console.WriteLine("destroying callback sender");
                _destroy = true;
                System.Threading.Monitor.Pulse(this);
            }
        }

        public void AddClient(ICallbackReceiverPrx? client, Current current)
        {
            lock (this)
            {
                Console.WriteLine($"adding client `{client!.Identity}'");
                _clients.Add(client.Clone(fixedConnection: current.Connection));
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

                        //
                        // Invoke callback on all clients; it's safe to do it with _mutex locked
                        // because Ice guarantees these async invocations never block the calling thread.
                        //
                        // The exception callback, if called, is called by an Ice thread, and never the
                        // calling thread.
                        foreach (ICallbackReceiverPrx c in _clients)
                        {
                            c.CallbackAsync(num).ContinueWith(t =>
                            {
                                if (t.Exception != null)
                                {
                                    RemoveClient(c, t.Exception.InnerException!);
                                }
                            });
                        }
                    }
                }
            }
        }

        private void RemoveClient(ICallbackReceiverPrx c, Exception ex)
        {
            lock (this)
            {
                if (_clients.Remove(c))
                {
                    Console.Error.WriteLine($"removing client `{c.Identity}':\n{ex}");
                }
            }
        }
    }
}
