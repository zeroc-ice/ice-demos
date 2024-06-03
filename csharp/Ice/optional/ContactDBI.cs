// Copyright (c) ZeroC, Inc.

using Demo;

#nullable enable

public class ContactDBI : ContactDBDisp_
{
    private Dictionary<string, Contact> _contacts = new Dictionary<string, Contact>();

    public override void addContact(string name, NumberType? type, string? number, int? dialGroup, Ice.Current current)
    {
        var contact = new Contact
        {
            name = name
        };
        if (type is not null)
        {
            contact.type = type;
        }
        if (number is not null)
        {
            contact.number = number;
        }
        if (dialGroup is not null)
        {
            contact.dialGroup = dialGroup;
        }
        _contacts[name] = contact;
    }

    public override void updateContact(
        string name,
        NumberType? type,
        string? number,
        int? dialGroup,
        Ice.Current current)
    {
        if (_contacts.TryGetValue(name, out Contact? c))
        {
            if (type is not null)
            {
                c.type = type;
            }
            if (number is not null)
            {
                c.number = number;
            }
            if (dialGroup is not null)
            {
                c.dialGroup = dialGroup;
            }
        }
    }

    public override Contact? query(string name, Ice.Current current)
    {
        if (_contacts.TryGetValue(name, out Contact? c))
        {
            return c;
        }
        return null;
    }

    public override string? queryNumber(string name, Ice.Current current)
    {
        if (_contacts.TryGetValue(name, out Contact? c))
        {
            return c.number;
        }
        return null;
    }

    public override void queryDialgroup(string name, out int? dialGroup, Ice.Current current)
    {
        if (_contacts.TryGetValue(name, out Contact? c))
        {
            dialGroup = c.dialGroup;
        }
        else
        {
            dialGroup = null;
        }
    }

    public override void shutdown(Ice.Current current)
    {
        System.Console.Out.WriteLine("Shutting down...");
        current.adapter.getCommunicator().shutdown();
    }
}
