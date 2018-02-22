#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import sys, Ice

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
        return 1

    adapter = communicator.createObjectAdapter("Callback.Client")
    adapter.add(CallbackReceiverI(), Ice.stringToIdentity("callbackReceiver"))
    adapter.activate()

    receiver = Demo.CallbackReceiverPrx.uncheckedCast(
        adapter.createProxy(Ice.stringToIdentity("callbackReceiver")))

    menu()

    c = None
    while c != 'x':
        try:
            sys.stdout.write("==> ")
            sys.stdout.flush()
            c = sys.stdin.readline().strip()
            if c == 't':
                sender.initiateCallback(receiver)
            elif c == 's':
                sender.shutdown()
            elif c == 'x':
                pass # Nothing to do
            elif c == '?':
                menu()
            else:
                print("unknown command `" + c + "'")
                menu()
        except EOFError:
            return 1
        except KeyboardInterrupt:
            return 1
    return 0

def menu():
    print("""
usage:
t: send callback
s: shutdown server
x: exit
?: help
""")

status = 0

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
        status = 1
    else:
        status = run(communicator)
sys.exit(status)
