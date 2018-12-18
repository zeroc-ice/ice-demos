#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import sys
import Ice

Ice.loadSlice('Session.ice')
import Demo


def run(communicator):
    while True:
        sys.stdout.write("Please enter your name ==> ")
        sys.stdout.flush()
        name = sys.stdin.readline().strip()
        if len(name) != 0:
            break

    base = communicator.propertyToProxy('SessionFactory.Proxy')
    factory = Demo.SessionFactoryPrx.checkedCast(base)
    if not factory:
        print(sys.args[0] + ": invalid proxy")
        sys.exit(1)
    session = factory.create(name)

    hellos = []

    menu()

    destroy = True
    shutdown = False
    while True:
        sys.stdout.write("==> ")
        sys.stdout.flush()
        c = sys.stdin.readline().strip()
        s = str(c)
        if s.isdigit():
            index = int(s)
            if index < len(hellos):
                hello = hellos[index]
                hello.sayHello()
            else:
                print("Index is too high. " + str(len(hellos)) + " hello objects exist so far.\n" +
                      "Use `c' to create a new hello object.")
        elif c == 'c':
            hellos.append(session.createHello())
            print("Created hello object", len(hellos) - 1)
        elif c == 's':
            destroy = False
            shutdown = True
            break
        elif c == 'x':
            break
        elif c == 't':
            destroy = False
            break
        elif c == '?':
            menu()
        else:
            print("unknown command `" + c + "'")
            menu()

    if destroy:
        session.destroy()
    if shutdown:
        factory.shutdown()


def menu():
    print("""
usage:
c:     create a new per-client hello object
0-9:   send a greeting to a hello object
s:     shutdown the server and exit
x:     exit
t:     exit without destroying the session
?:     help
""")


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
        sys.exit(1)

    run(communicator)
