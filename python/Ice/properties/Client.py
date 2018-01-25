#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import sys, traceback, Ice

slice_dir = Ice.getSliceDir()
if not slice_dir:
    print(sys.argv[0] + ': Slice directory not found.')
    sys.exit(1)

Ice.loadSlice("'-I" + slice_dir + "' Props.ice")
import Demo

def run(communicator):
    props = Demo.PropsPrx.checkedCast(communicator.propertyToProxy("Props.Proxy"))
    if props == None:
        print("invalid proxy")
        return 1

    admin = Ice.PropertiesAdminPrx.checkedCast(communicator.propertyToProxy("Admin.Proxy"))

    batch1 = {}
    batch1["Demo.Prop1"] = "1"
    batch1["Demo.Prop2"] = "2"
    batch1["Demo.Prop3"] = "3"

    batch2 = {}
    batch2["Demo.Prop1"] = "10"
    batch2["Demo.Prop2"] = "" # An empty value removes this property
    batch2["Demo.Prop3"] = "30"

    show(admin)
    menu()

    c = None
    while c != 'x':
        try:
            sys.stdout.write("==> ")
            sys.stdout.flush()
            c = sys.stdin.readline().strip()
            if c == "1" or c == "2":
                propsDict = c == "1" and batch1 or batch2
                print("Sending:")
                for k, v in propsDict.items():
                    if k.startswith("Demo"):
                        print("  " + k + "=" + v)
                    print

                    admin.setProperties(propsDict)

                    print("Changes:")
                    changes = props.getChanges()
                    if len(changes) == 0:
                        print("  None.")
                    else:
                        for k, v in changes.items():
                            sys.stdout.write("  " + k)
                            if len(v) == 0:
                                print(" was removed")
                            else:
                                print(" is now " + v)
            elif c == "c":
                show(admin)
            elif c == "s":
                props.shutdown()
            elif c == "x":
                # Nothing to do
                pass
            elif c == "?":
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
1: set properties (batch 1)
2: set properties (batch 2)
c: show current properties
s: shutdown server
x: exit
?: help
""")

def show(admin):
    props = admin.getPropertiesForPrefix("Demo")
    print "Server's current settings:"
    for k,v in props.items():
        print "  " + k + "=" + v

status = 0
with Ice.initialize(sys.argv, "config.client") as communicator:
    if len(sys.argv) > 1:
        print(sys.argv[0] + ": too many arguments")
        status = 1
    else:
        status = run(communicator)
sys.exit(status)
