#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import sys, traceback, Ice, IceGrid

Ice.loadSlice('Hello.ice')
import Demo

def run(communicator):
    hello = None
    try:
        hello = Demo.HelloPrx.checkedCast(communicator.stringToProxy("hello"))
    except Ice.NotRegisteredException:
        query = IceGrid.QueryPrx.checkedCast(communicator.stringToProxy("DemoIceGrid/Query"))
        hello = Demo.HelloPrx.checkedCast(query.findObjectByType("::Demo::Hello"))

    if not hello:
        print("couldn't find a `::Demo::Hello' object.")
        return 1

    menu()

    c = None
    while c != 'x':
        try:
            sys.stdout.write("==> ")
            sys.stdout.flush()
            c = sys.stdin.readline().strip()
            if c == 't':
                hello.sayHello()
            elif c == 's':
                hello.shutdown()
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
t: send greeting as twoway
s: shutdown server
x: exit
?: help
""")

status = 0

#
# Ice.initialize returns an initialized Ice communicator,
# the communicator is destroyed once it goes out of scope.
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
