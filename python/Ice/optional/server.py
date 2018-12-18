#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import signal
import sys
import Ice

Ice.loadSlice("Contact.ice")
import Demo


class ContactDBI(Demo.ContactDB):
    def __init__(self):
        self._contacts = {}

    def addContact(self, name, type, number, dialGroup, current=None):
        contact = Demo.Contact()
        contact.name = name
        if type != Ice.Unset:
            contact.type = type
        if number != Ice.Unset:
            contact.number = number
        if dialGroup != Ice.Unset:
            contact.dialGroup = dialGroup
        self._contacts[name] = contact

    def updateContact(self, name, type, number, dialGroup, current=None):
        if name in self._contacts:
            contact = self._contacts[name]
            if type != Ice.Unset:
                contact.type = type
            if number != Ice.Unset:
                contact.number = number
            if dialGroup != Ice.Unset:
                contact.dialGroup = dialGroup

    def query(self, name, current=None):
        if name in self._contacts:
            return self._contacts[name]
        return None

    def queryNumber(self, name, current=None):
        if name in self._contacts:
            return self._contacts[name].number
        return Ice.Unset

    def queryDialgroup(self, name, current=None):
        if name in self._contacts:
            return self._contacts[name].dialGroup
        return Ice.Unset

    def shutdown(self, current=None):
        print("Shutting down...")
        current.adapter.getCommunicator().shutdown()


#
# Ice.initialize returns an initialized Ice communicator,
# the communicator is destroyed once it goes out of scope.
#
with Ice.initialize(sys.argv, "config.server") as communicator:

    #
    # Install a signal handler to shutdown the communicator on Ctrl-C
    #
    signal.signal(signal.SIGINT, lambda signum, handler: communicator.shutdown())

    #
    # The communicator initialization removes all Ice-related arguments from argv
    #
    if len(sys.argv) > 1:
        print(sys.argv[0] + ": too many arguments")
        sys.exit(1)

    adapter = communicator.createObjectAdapter("ContactDB")
    adapter.add(ContactDBI(), Ice.stringToIdentity("contactdb"))
    adapter.activate()
    communicator.waitForShutdown()
