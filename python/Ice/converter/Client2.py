#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import sys, traceback, Ice

Ice.loadSlice('Greet.ice')
import Demo

def menu():
    print("""
usage:
t: send greeting
s: shutdown server
x: exit
?: help
""")

def decodeString(str):
    ret = ""
    for i in range(0, len(str)):
        n = ord(str[i])
        if n < 32 or n > 126:
            ret += "\\" + oct(n)[1:] # Skip leading '0'
        else:
            ret += str[i]
    return ret

communicator = None

class Client:
    def run(self, args):
        if len(args) > 1:
            print(self.appName() + ": too many arguments")
            return 1

        greet = Demo.GreetPrx.checkedCast(communicator.propertyToProxy('Greet.Proxy'))
        if not greet:
            print(args[0] + ": invalid proxy")
            return 1
        
        menu()

        greeting = "Bonne journ\351e";

        c = None
        while c != 'x':
            try:
                sys.stdout.write("==> ")
                sys.stdout.flush()
                c = sys.stdin.readline().strip()
                if c == 't':
                    ret = greet.exchangeGreeting(greeting)
                    print("Received by client: \"" + decodeString(ret) + "\"")
                elif c == 's':
                    greet.shutdown()
                elif c == 'x':
                    pass # Nothing to do
                elif c == '?':
                    menu()
                else:
                    print("unknown command `" + c + "'")
                    menu()
            except KeyboardInterrupt:
                break
            except EOFError:
                break
            except Ice.Exception as ex:
                print(ex)

        return 0

status = 0

try:

    initData = Ice.InitializationData()
    initData.properties = Ice.createProperties(None, initData.properties)
    initData.properties.load("config.client")

    communicator = Ice.initialize(sys.argv, initData)

    app = Client()
    status = app.run(sys.argv)

except:
    traceback.print_exc()
    status = 1

if communicator:
    try:
        communicator.destroy()
    except:
        traceback.print_exc()
        status = 1

sys.exit(status)
