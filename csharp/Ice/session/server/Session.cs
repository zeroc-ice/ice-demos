// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using System.Collections.Generic;
using ZeroC.Ice;

namespace Demo
{
    public class Session : ISession
    {
        private bool _destroy;
        private readonly object _mutex = new object();
        private readonly string _name;
        private int _nextId; // The per-session id of the next hello object. This is used for tracing purposes.
        private readonly List<IHelloPrx> _objs = new List<IHelloPrx>(); // List of per-session allocated hello objects.

        public Session(string name)
        {
            _name = name;
            _nextId = 0;
            _destroy = false;
            Console.Out.WriteLine($"The session {_name} is now created.");
        }

        public IHelloPrx CreateHello(Current current)
        {
            lock (_mutex)
            {
                if (_destroy)
                {
                    throw new ObjectNotExistException(current);
                }

                IHelloPrx hello = current.Adapter.AddWithUUID(new Hello(_name, _nextId++), IHelloPrx.Factory);
                _objs.Add(hello);
                return hello;
            }
        }

        public string GetName(Current current)
        {
            lock (_mutex)
            {
                if (_destroy)
                {
                    throw new ObjectNotExistException(current);
                }

                return _name;
            }
        }

        public void Destroy(Current current)
        {
            lock (_mutex)
            {
                if (_destroy)
                {
                    throw new ObjectNotExistException(current);
                }

                _destroy = true;

                Console.WriteLine($"The session {_name} is now destroyed.");
                try
                {
                    current.Adapter.Remove(current.Identity);
                    foreach (IHelloPrx p in _objs)
                    {
                        current.Adapter.Remove(p.Identity);
                    }
                }
                catch (ObjectDisposedException)
                {
                    // This method is called on shutdown of the server, in which
                    // case this exception is expected.
                }
            }

            _objs.Clear();
        }
    }

}
