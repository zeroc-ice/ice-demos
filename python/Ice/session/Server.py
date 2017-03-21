#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import sys, threading, traceback, Ice

Ice.loadSlice('Session.ice')
import Demo

class HelloI(Demo.Hello):
    def __init__(self, name, id):
        self._name = name
        self._id = id

    def sayHello(self, c):
        print("Hello object #" + str(self._id) + " for session `" + self._name + "' says:\n" + \
              "Hello " + self._name + "!")


class SessionI(Demo.Session):
    def __init__(self, name):
        self._name = name
        self._lock = threading.Lock()
        self._destroy = False # true if destroy() was called, false otherwise.
        self._nextId = 0 # The id of the next hello object. This is used for tracing purposes.
        self._objs = [] # List of per-client allocated Hello objects.

        print("The session " + self._name + " is now created.")

    def createHello(self, current):
        self._lock.acquire()
        try:
            if self._destroy:
                raise Ice.ObjectNotExistException()

            hello = Demo.HelloPrx.uncheckedCast(current.adapter.addWithUUID(HelloI(self._name, self._nextId)))
            self._nextId = self._nextId + 1
            self._objs.append(hello)
            return hello
        finally:
            self._lock.release()

    def getName(self, current):
        self._lock.acquire()
        try:
            if self._destroy:
                raise Ice.ObjectNotExistException()
            return self._name
        finally:
            self._lock.release()

    def destroy(self, current):
        self._lock.acquire()
        try:
            if self._destroy:
                raise Ice.ObjectNotExistException()
            self._destroy = True
            print("The session " + self._name + " is now destroyed.")
            try:
                current.adapter.remove(current.id)
                for p in self._objs:
                    current.adapter.remove(p.ice_getIdentity())
            except Ice.ObjectAdapterDeactivatedException as ex:
                # This method is called on shutdown of the server, in
                # which case this exception is expected.
                pass
            self._objs = []
        finally:
            self._lock.release()


class SessionFactoryI(Demo.SessionFactory):
    def create(self, name, current):

        class CloseCallbackI(Ice.ConnectionCallback):
            def __init__(self, session):
                self._session = session

            def heartbeat(self, con):
                pass # ignored

            def closed(self, con):
                try:
                    self._session.destroy()
                    print("Cleaned up dead client.")
                except Ice.LocalException as ex:
                    # The client already destroyed this session, or the server is shutting down
                    pass

        session = SessionI(name)
        proxy = Demo.SessionPrx.uncheckedCast(current.adapter.addWithUUID(session))

        #
        # Remove endpoints to ensure that calls are collocated-only
        # This way, if we invoke on the proxy during shutdown, the invocation fails immediately
        # without attempting to establish any connection
        #
        collocProxy = proxy.ice_endpoints([])

        #
        # Never close this connection from the client and turn on heartbeats with a timeout of 30s
        #
        current.con.setACM(30, Ice.ACMClose.CloseOff, Ice.ACMHeartbeat.HeartbeatAlways)
        current.con.setCallback(CloseCallbackI(collocProxy))
        return proxy

    def shutdown(self, current):
        print("Shutting down...")
        current.adapter.getCommunicator().shutdown()

class Server(Ice.Application):
    def run(self, args):
        if len(args) > 1:
            print(self.appName() + ": too many arguments")
            return 1

        adapter = self.communicator().createObjectAdapter("SessionFactory")
        adapter.add(SessionFactoryI(), Ice.stringToIdentity("SessionFactory"))
        adapter.activate()
        self.communicator().waitForShutdown()
        return 0

app = Server()
sys.exit(app.main(sys.argv, "config.server"))
