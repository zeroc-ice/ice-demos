#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import signal
import sys
import time
import Ice

Ice.loadSlice('Latency.ice')
import Demo


def run(communicator):
    ping = Demo.PingPrx.checkedCast(communicator.propertyToProxy('Ping.Proxy'))
    if not ping:
        print("invalid proxy")
        sys.exit(1)

    # Initial ping to setup the connection.
    ping.ice_ping()

    repetitions = 100000
    print("pinging server " + str(repetitions) + " times (this may take a while)")

    tsec = time.time()

    i = repetitions
    while(i >= 0):
        ping.ice_ping()
        i = i - 1

    tsec = time.time() - tsec
    tmsec = tsec * 1000.0

    print("time for %d pings: %.3fms" % (repetitions, tmsec))
    print("time per ping: %.3fms" % (tmsec / repetitions))


#
# Ice.initialize returns an initialized Ice communicator,
# the communicator is destroyed once it goes out of scope.
#
with Ice.initialize(sys.argv, "config.client") as communicator:

    #
    # Install a signal handler to destroy the communicator on Ctrl-C
    #
    signal.signal(signal.SIGINT, lambda signum, handler: communicator.destroy())

    #
    # The communicator initialization removes all Ice-related arguments from argv
    #
    if len(sys.argv) > 1:
        print(sys.argv[0] + ": too many arguments")
        sys.exit(1)

    run(communicator)
