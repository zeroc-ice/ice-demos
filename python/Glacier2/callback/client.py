#!/usr/bin/env python
#
# Copyright (c) ZeroC, Inc. All rights reserved.
#

import sys
import Ice
import Glacier2
Ice.loadSlice('Callback.ice')
import Demo
import uuid

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
    #
    # Loop until we have successfully create a session.
    #
    while not session:
        #
        # Prompt the user for the credentials to create the session.
        #
        print("This demo accepts any user-id / password combination.")
        sys.stdout.write("user id: ")
        sys.stdout.flush()
        id = sys.stdin.readline().strip()
        sys.stdout.write("password: ")
        sys.stdout.flush()
        pw = sys.stdin.readline().strip()

        #
        # Try to create a session and break the loop if succeed,
        # otherwise try again after printing the error message.
        #
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

    #
    # The Glacier2 router routes bidirectional calls to objects in the client only
    # when these objects have the correct Glacier2-issued category. The purpose of
    # the callbackReceiverFakeIdent is to demonstrate this.
    #
    # The Identity name is not checked by the server any value can be used.
    #
    callbackReceiverIdent = Ice.Identity("callbackReceiver", router.getCategoryForClient())
    callbackReceiverFakeIdent = Ice.Identity(str(uuid.uuid4()), "fake")

    base = communicator.propertyToProxy('Callback.Proxy')
    twoway = Demo.CallbackPrx.checkedCast(base)
    oneway = Demo.CallbackPrx.uncheckedCast(twoway.ice_oneway())
    batchOneway = Demo.CallbackPrx.uncheckedCast(twoway.ice_batchOneway())

    adapter = communicator.createObjectAdapterWithRouter("", router)
    adapter.add(CallbackReceiverI(), callbackReceiverIdent)

    #
    # Callback will never be called for a fake identity.
    #
    adapter.add(CallbackReceiverI(), callbackReceiverFakeIdent)

    twowayR = Demo.CallbackReceiverPrx.uncheckedCast(adapter.createProxy(callbackReceiverIdent))
    onewayR = Demo.CallbackReceiverPrx.uncheckedCast(twowayR.ice_oneway())

    override = None
    fake = False

    menu()

    #
    # Client REPL
    #
    c = None
    while c != 'x':
        try:
            sys.stdout.write("==> ")
            sys.stdout.flush()
            c = sys.stdin.readline().strip()
            if c == 't':
                twoway.initiateCallback(twowayR)
            elif c == 'o':
                oneway.initiateCallback(onewayR, {"_ovrd": override} if override else {})
            elif c == 'O':
                context = {}
                context["_fwd"] = "O"
                if override:
                    context["_ovrd"] = override
                batchOneway.initiateCallback(onewayR, context)
            elif c == 'f':
                batchOneway.ice_flushBatchRequests()
            elif c == 'v':
                if not override:
                    override = "some_value"
                    print("override context field is now `" + override + "'")
                else:
                    override = None
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
