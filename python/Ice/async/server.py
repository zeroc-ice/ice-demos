#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import signal
import sys
import threading
import Ice

Ice.loadSlice('Hello.ice')
import Demo


class CallbackEntry(object):
    def __init__(self, f, delay):
        self.future = f
        self.delay = delay


class WorkQueue(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
        self._callbacks = []
        self._done = False
        self._cond = threading.Condition()

    def run(self):
        with self._cond:
            while not self._done:
                if len(self._callbacks) == 0:
                    self._cond.wait()

                if not len(self._callbacks) == 0:
                    self._cond.wait(self._callbacks[0].delay / 1000.0)

                    if not self._done:
                        print("Belated Hello World!")
                        self._callbacks[0].future.set_result(None)
                        del self._callbacks[0]

            for i in range(0, len(self._callbacks)):
                self._callbacks[i].future.set_exception(Demo.RequestCanceledException())

    def add(self, delay):
        future = Ice.Future()
        with self._cond:
            if not self._done:
                entry = CallbackEntry(future, delay)
                if len(self._callbacks) == 0:
                    self._cond.notify()
                self._callbacks.append(entry)
            else:
                future.set_exception(Demo.RequestCanceledException())
        return future

    def destroy(self):
        with self._cond:
            self._done = True
            self._cond.notify()


class HelloI(Demo.Hello):
    def __init__(self, workQueue):
        self._workQueue = workQueue

    def sayHello(self, delay, current):
        if delay == 0:
            print("Hello World!")
            return None
        else:
            return self._workQueue.add(delay)

    def shutdown(self, current):
        self._workQueue.destroy()
        current.adapter.getCommunicator().shutdown()


def interruptHandler(signum, frame, communicator, workQueue):
    workQueue.destroy()
    communicator.shutdown()


#
# Ice.initialize returns an initialized Ice communicator,
# the communicator is destroyed once it goes out of scope.
#
with Ice.initialize(sys.argv, "config.server") as communicator:

    #
    # Install a signal handler to shutdown the communicator on Ctrl-C
    #
    signal.signal(signal.SIGINT, lambda signum, frame: interruptHandler(signum, frame, communicator, workQueue))

    #
    # The communicator initialization removes all Ice-related arguments from argv
    #
    if len(sys.argv) > 1:
        print(sys.argv[0] + ": too many arguments")
        sys.exit(1)

    adapter = communicator.createObjectAdapter("Hello")
    workQueue = WorkQueue()
    adapter.add(HelloI(workQueue), Ice.stringToIdentity("hello"))

    workQueue.start()
    adapter.activate()

    communicator.waitForShutdown()
    workQueue.join()
