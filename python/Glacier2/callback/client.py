#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import sys
import Ice
import Glacier2
Ice.loadSlice('Callback.ice')
import Demo


def menu():
    print("""
usage:
t: send callback as twoway
o: send callback as oneway
O: send callback as batch oneway
f: flush all batch requests
v: set/reset override context field
F: set/reset fake category
s: shutdown server
x: exit
?: help
""")


class CallbackReceiverI(Demo.CallbackReceiver):
    def callback(self, current):
        print("received callback")


def run(communicator):
    router = Glacier2.RouterPrx.checkedCast(communicator.getDefaultRouter())
    session = None
    while not session:
        print("This demo accepts any user-id / password combination.")
        sys.stdout.write("user id: ")
        sys.stdout.flush()
        id = sys.stdin.readline().strip()
        sys.stdout.write("password: ")
        sys.stdout.flush()
        pw = sys.stdin.readline().strip()
        try:
            session = router.createSession(id, pw)
            break
        except Glacier2.PermissionDeniedException as ex:
            print("permission denied:\n" + ex.reason)
        except Glacier2.CannotCreateSessionException as ex:
            print("cannot create session:\n" + ex.reason)

    acmTimeout = router.getACMTimeout()
    connection = router.ice_getCachedConnection()
    assert(connection)
    connection.setACM(acmTimeout, Ice.Unset, Ice.ACMHeartbeat.HeartbeatAlways)
    # Note: we use sys.stdout.write instead of print because print can't be used in a lambda with Python 2.x
    connection.setCloseCallback(lambda conn: sys.stdout.write("The Glacier2 session has been destroyed.\n"))

    callbackReceiverIdent = Ice.Identity()
    callbackReceiverIdent.name = "callbackReceiver"
    callbackReceiverIdent.category = router.getCategoryForClient()

    callbackReceiverFakeIdent = Ice.Identity()
    callbackReceiverFakeIdent.name = "callbackReceiver"
    callbackReceiverFakeIdent.category = "fake"

    base = communicator.propertyToProxy('Callback.Proxy')
    twoway = Demo.CallbackPrx.checkedCast(base)
    oneway = Demo.CallbackPrx.uncheckedCast(twoway.ice_oneway())
    batchOneway = Demo.CallbackPrx.uncheckedCast(twoway.ice_batchOneway())

    adapter = communicator.createObjectAdapterWithRouter("", router)
    adapter.add(CallbackReceiverI(), callbackReceiverIdent)

    # Should never be called for the fake identity.
    adapter.add(CallbackReceiverI(), callbackReceiverFakeIdent)

    twowayR = Demo.CallbackReceiverPrx.uncheckedCast(adapter.createProxy(callbackReceiverIdent))
    onewayR = Demo.CallbackReceiverPrx.uncheckedCast(twowayR.ice_oneway())

    override = ''
    fake = False

    menu()

    c = None
    while c != 'x':
        try:
            sys.stdout.write("==> ")
            sys.stdout.flush()
            c = sys.stdin.readline().strip()
            if c == 't':
                context = {}
                context["_fwd"] = "t"
                if not len(override) == 0:
                    context["_ovrd"] = override
                twoway.initiateCallback(twowayR, context)
            elif c == 'o':
                context = {}
                context["_fwd"] = "o"
                if not len(override) == 0:
                    context["_ovrd"] = override
                oneway.initiateCallback(onewayR, context)
            elif c == 'O':
                context = {}
                context["_fwd"] = "O"
                if not len(override) == 0:
                    context["_ovrd"] = override
                batchOneway.initiateCallback(onewayR, context)
            elif c == 'f':
                batchOneway.ice_flushBatchRequests()
            elif c == 'v':
                if len(override) == 0:
                    override = "some_value"
                    print("override context field is now `" + override + "'")
                else:
                    override = ''
                    print("override context field is empty")
            elif c == 'F':
                fake = not fake

                if fake:
                    twowayR = Demo.CallbackReceiverPrx.uncheckedCast(
                        twowayR.ice_identity(callbackReceiverFakeIdent))
                    onewayR = Demo.CallbackReceiverPrx.uncheckedCast(
                        onewayR.ice_identity(callbackReceiverFakeIdent))
                else:
                    twowayR = Demo.CallbackReceiverPrx.uncheckedCast(twowayR.ice_identity(callbackReceiverIdent))
                    onewayR = Demo.CallbackReceiverPrx.uncheckedCast(twowayR.ice_identity(callbackReceiverIdent))
            elif c == 's':
                twoway.shutdown()
            elif c == 'x':
                pass  # Nothing to do
            elif c == '?':
                menu()
            else:
                print("unknown command `" + c + "'")
                menu()
        except KeyboardInterrupt:
            break
        except EOFError:
            break

    return 0


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
