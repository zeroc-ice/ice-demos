#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import os, sys, threading, Ice


slice_dir = Ice.getSliceDir()
if not slice_dir:
    print(sys.argv[0] + ': Slice directory not found.')
    sys.exit(1)

Ice.loadSlice("'-I" + slice_dir + "' Callback.ice")
import Demo

class CallbackSenderI(Demo.CallbackSender, threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
        self._destroy = False
        self._clients = []
        self._cond = threading.Condition()

    def destroy(self):
        with self._cond:
            print("destroying callback sender")
            self._destroy = True
            self._cond.notify()

        self.join()

    def addClient(self, ident, current=None):
        with self._cond:
            print("adding client `" + Ice.identityToString(ident) + "'")
            client = Demo.CallbackReceiverPrx.uncheckedCast(current.con.createProxy(ident))
            self._clients.append(client)

    def run(self):
        num = 0
        with self._cond:
            while not self._destroy:
                self._cond.wait(2)

                if not self._destroy and len(self._clients) > 0:
                    num = num + 1
                    #
                    # Call clients within synchronization. This is safe because (a) Ice guarantees
                    # these async invocations never block and (b) add_done_callback_async on an
                    # Ice.InvocationFuture returned by callbackAsync always executes the done callback
                    # in an Ice thread.
                    #
                    for p in self._clients:
                        p.callbackAsync(num).add_done_callback_async(
                            lambda f, client = p: self.removeClient(client, f.exception()) if f.exception() else None)

    def removeClient(self, client, ex):
        with self._cond:
            print("removing client `" + Ice.identityToString(client.ice_getIdentity()) + "':\n" + str(ex))
            self._clients.remove(client)


class Server(Ice.Application):
    def run(self, args):
        if len(args) > 1:
            print(self.appName() + ": too many arguments")
            return 1

        adapter = self.communicator().createObjectAdapter("Callback.Server")
        sender = CallbackSenderI()
        adapter.add(sender, Ice.stringToIdentity("sender"))
        adapter.activate()

        sender.start()
        self.communicator().waitForShutdown()
        sender.destroy()

        return 0

app = Server()
sys.exit(app.main(sys.argv, "config.server"))
