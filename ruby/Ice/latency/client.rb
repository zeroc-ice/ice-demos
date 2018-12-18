#!/usr/bin/env ruby
# **********************************************************************
#
# Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
#
# **********************************************************************

require 'Ice'

Ice::loadSlice('Latency.ice')

def run(communicator, args)
    if args.length > 0
        puts $0 + ": too many argumnets"
        return 1
    end

    ping = Demo::PingPrx::checkedCast(communicator.propertyToProxy('Ping.Proxy'))
    if not ping
        puts "invalid proxy"
        return 1
    end

    # Initial ping to setup the connection.
    ping.ice_ping()

    repetitions = 100000
    puts "pinging server " + repetitions.to_s + " times (this may take a while)"

    tsec = Time.now

    i = repetitions
    while i >= 0
        ping.ice_ping()
        i = i - 1
    end

    tsec = Time.now - tsec
    tmsec = tsec * 1000.0

    printf "time for %d pings: %.3fms\n", repetitions, tmsec
    printf "time per ping: %.3fms\n", tmsec / repetitions

    return 0
end

status = Ice::initialize(ARGV, "config.client") { |communicator, args| run(communicator, args) }
exit(status)
