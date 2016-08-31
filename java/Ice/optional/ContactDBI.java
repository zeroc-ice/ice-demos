// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

public class ContactDBI implements ContactDB
{
    private java.util.Map<String, Contact> _contacts = new java.util.HashMap<>();

    @Override
    public final void addContact(String name, java.util.Optional<NumberType> type, java.util.Optional<String> number,
                                 java.util.OptionalInt dialGroup, com.zeroc.Ice.Current current)
    {
        Contact contact = new Contact();
        contact.name = name;
        if(type.isPresent())
        {
            contact.setType(type.get());
        }
        if(number.isPresent())
        {
            contact.setNumber(number.get());
        }
        if(dialGroup.isPresent())
        {
            contact.setDialGroup(dialGroup.getAsInt());
        }
        _contacts.put(name, contact);
    }

    @Override
    public final void updateContact(String name, java.util.Optional<NumberType> type,
                                    java.util.Optional<String> number, java.util.OptionalInt dialGroup,
                                    com.zeroc.Ice.Current current)
    {
        Contact c = _contacts.get(name);
        if(c != null)
        {
            if(type.isPresent())
            {
                c.setType(type.get());
            }
            if(number.isPresent())
            {
                c.setNumber(number.get());
            }
            if(dialGroup.isPresent())
            {
                c.setDialGroup(dialGroup.getAsInt());
            }
        }
    }

    @Override
    public final Contact query(String name, com.zeroc.Ice.Current current)
    {
        return _contacts.get(name);
    }

    @Override
    public final java.util.OptionalInt queryDialgroup(String name, com.zeroc.Ice.Current current)
    {
        Contact c = _contacts.get(name);
        if(c != null)
        {
            return c.optionalDialGroup();
        }
        return java.util.OptionalInt.empty();
    }

    @Override
    public final java.util.Optional<String> queryNumber(String name, com.zeroc.Ice.Current current)
    {
        java.util.Optional<String> ret = null;
        Contact c = _contacts.get(name);
        if(c != null)
        {
            ret = c.optionalNumber();
        }
        return ret;
    }

    @Override
    public void shutdown(com.zeroc.Ice.Current current)
    {
        System.out.println("Shutting down...");
        current.adapter.getCommunicator().shutdown();
    }
}
