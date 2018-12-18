#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import signal
import sys
import Ice

Ice.loadSlice('Fetcher.ice')
import Demo


def run(communicator):

    fetcher = Demo.FetcherPrx.checkedCast(communicator.propertyToProxy('Fetcher.Proxy'))
    if not fetcher:
        print("invalid proxy")
        sys.exit(1)

    content = fetcher.fetch('http://zeroc.com')
    print("HTTP headers from zeroc.com:")
    print(content)

    fetcher.shutdown()


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
