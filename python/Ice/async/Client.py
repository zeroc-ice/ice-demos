#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import sys, traceback, Ice

Ice.loadSlice('Hello.ice')
import Demo

class Callback:
    def response(self, f):
        try:
            f.result()
        except Demo.RequestCanceledException:
            print("Demo.RequestCanceledException")
        except:
            print("sayHello AMI call failed:")
            traceback.print_exc()

def run(communicator):
    hello = Demo.HelloPrx.checkedCast(communicator.propertyToProxy('Hello.Proxy'))
    if not hello:
        print(sys.argv[0] + ": invalid proxy")
        return 1

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
        except Ice.Exception as ex:
            print(ex)
    return 0

def menu():
    print("""
usage:
i: send immediate greeting
d: send delayed greeting
s: shutdown server
x: exit
?: help
""")

status = 0
with Ice.initialize(sys.argv, "config.client") as communicator:
    if len(sys.argv) > 1:
        print(sys.argv[0] + ": too many arguments")
        status = 1
    else:
        status = run(communicator)
sys.exit(status)
