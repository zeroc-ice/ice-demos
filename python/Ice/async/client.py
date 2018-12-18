#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import sys
import traceback
import Ice

Ice.loadSlice('Hello.ice')
import Demo


class Callback:
    def response(self, f):
        try:
            f.result()
        except Demo.RequestCanceledException:
            print("Demo.RequestCanceledException")
        except Exception:
            print("sayHello AMI call failed:")
            traceback.print_exc()


def run(communicator):
    hello = Demo.HelloPrx.checkedCast(communicator.propertyToProxy('Hello.Proxy'))
    if not hello:
        print(sys.args[0] + ": invalid proxy")
        sys.exit(1)

    menu()

    c = None
    while c != 'x':
        try:
            sys.stdout.write("==> ")
            sys.stdout.flush()
            c = sys.stdin.readline().strip()
            if c == 'i':
                hello.sayHello(0)
            elif c == 'd':
                cb = Callback()
                hello.sayHelloAsync(5000).add_done_callback(cb.response)
            elif c == 's':
                hello.shutdown()
            elif c == 'x':
                pass  # Nothing to do
            elif c == '?':
                menu()
            else:
                print("unknown command `" + c + "'")
                menu()
        except Ice.Exception as ex:
            print(ex)


def menu():
    print("""
usage:
i: send immediate greeting
d: send delayed greeting
s: shutdown server
x: exit
?: help
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
