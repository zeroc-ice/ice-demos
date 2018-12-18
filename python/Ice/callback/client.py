#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import sys
import Ice

Ice.loadSlice('Callback.ice')
import Demo


class CallbackReceiverI(Demo.CallbackReceiver):
    def callback(self, current):
        print("received callback")


def run(communicator):
    sender = Demo.CallbackSenderPrx.checkedCast(
        communicator.propertyToProxy('CallbackSender.Proxy').
        ice_twoway().ice_timeout(-1).ice_secure(False))
    if not sender:
        print("invalid proxy")
        sys.exit(1)

    adapter = communicator.createObjectAdapter("Callback.Client")
    adapter.add(CallbackReceiverI(), Ice.stringToIdentity("callbackReceiver"))
    adapter.activate()

    receiver = Demo.CallbackReceiverPrx.uncheckedCast(
        adapter.createProxy(Ice.stringToIdentity("callbackReceiver")))

    menu()

    c = None
    while c != 'x':
        sys.stdout.write("==> ")
        sys.stdout.flush()
        c = sys.stdin.readline().strip()
        if c == 't':
            sender.initiateCallback(receiver)
        elif c == 's':
            sender.shutdown()
        elif c == 'x':
            pass  # Nothing to do
        elif c == '?':
            menu()
        else:
            print("unknown command `" + c + "'")
            menu()


def menu():
    print("""
usage:
t: send callback
s: shutdown server
x: exit
?: help
""")


#
# The Ice communicator is initlialized with Ice.initialize
# The communicator is destroyed once it goes out of scope
#
with Ice.initialize(sys.argv, "config.client") as communicator:

    #
    # The communicator initialization removes all Ice-related arguments from argv
    #
    if len(sys.argv) > 1:
        print(sys.argv[0] + ": too many arguments")
        sys.exit(1)
    run(communicator)
