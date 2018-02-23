#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import sys, Ice

Ice.loadSlice('Context.ice')
import Demo

def run(communicator):
    proxy = Demo.ContextPrx.checkedCast(communicator.propertyToProxy('Context.Proxy'))
    if not proxy:
        print(sys.args[0] + ": invalid proxy")
        return 1

    menu()

    c = None
    while c != 'x':
        try:
            sys.stdout.write("==> ")
            sys.stdout.flush()
            c = sys.stdin.readline().strip()
            if c == '1':
                proxy.call()
            elif c == '2':
                ctx = {'type': 'Explicit'}
                proxy.call(ctx)
            elif c == '3':
                ctx = {'type': 'Per-Proxy'}
                proxy2 = Demo.ContextPrx.uncheckedCast(proxy.ice_context(ctx))
                proxy2.call()
            elif c == '4':
                ic = communicator.getImplicitContext()
                ctx = {'type': 'Implicit'}
                ic.setContext(ctx)
                proxy.call()
                ic.setContext({})
            elif c == 's':
                proxy.shutdown()
            elif c == 'x':
                pass  # Nothing to do
            elif c == '?':
                menu()
            else:
                print("unknown command `" + c + "'")
                menu()
        except KeyboardInterrupt:
            return 1
        except EOFError:
            return 1
        except Ice.Exception as ex:
            print(ex)
    return 0

def menu():
    print("""
usage:
1: call with no request context
2: call with explicit request context
3: call with per-proxy request context
4: call with implicit request context
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
