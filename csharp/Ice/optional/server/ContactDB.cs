// Copyright (c) ZeroC, Inc. All rights reserved.

using System.Collections.Generic;
using ZeroC.Ice;

namespace Demo
{
    public class ContactDB : IContactDB
    {
        readonly Dictionary<string, Contact> _contacts = new Dictionary<string, Contact>();

        public void AddContact(string name, NumberType? type, string? number, int? dialGroup, Current current)
        {
            var contact = new Contact(name, type, number, dialGroup);
            _contacts[name] = contact;
        }

        public void UpdateContact(string name, NumberType? type, string? number, int? dialGroup, Current current)
        {
            if (_contacts.TryGetValue(name, out Contact? c))
            {
                if (type.HasValue)
                {
                    c.Type = type;
                }

                if (number != null)
                {
                    c.Number = number;
                }

                if (dialGroup.HasValue)
                {
                    c.DialGroup = dialGroup;
                }
            }
        }

        public Contact? Query(string name, Current current)
        {
            if (_contacts.TryGetValue(name, out Contact? c))
            {
                return c;
            }
            return null;
        }

        public string? QueryNumber(string name, Current current)
        {
            if (_contacts.TryGetValue(name, out Contact? c))
            {
                return c.Number;
            }
            return null;
        }

        public int? QueryDialgroup(string name, Current current)
        {
            if (_contacts.TryGetValue(name, out Contact? c))
            {
                return c.DialGroup;
            }
            return null;
        }

        public void Shutdown(Current current)
        {
            System.Console.WriteLine("Shutting down...");
            current.Communicator.DisposeAsync();
        }
    }
}
