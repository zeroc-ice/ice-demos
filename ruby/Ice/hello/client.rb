#!/usr/bin/env ruby
# **********************************************************************
#
# Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
#
# **********************************************************************

require 'Ice'

Ice::loadSlice('Hello.ice')

def run(communicator, args)
    if args.length > 0
        puts $0 + ": too many arguments"
        return 1
    end

    twoway = Demo::HelloPrx::checkedCast(
    communicator.propertyToProxy('Hello.Proxy').ice_twoway().ice_secure(false))
    if not twoway
        puts $0 + ": invalid proxy"
        return 1
    end

    oneway = twoway.ice_oneway()
    batchOneway = twoway.ice_batchOneway()
    datagram = twoway.ice_datagram()
    batchDatagram = twoway.ice_batchDatagram()

    secure = false
    timeout = -1
    delay = 0

    menu()

    c = nil
    while c != 'x'
        begin
            print "==> "
            STDOUT.flush
            line = STDIN.readline
            c = line[0..0]
            if c == 't'
                twoway.sayHello(delay)
            elsif c == 'o'
                oneway.sayHello(delay)
            elsif c == 'O'
                batchOneway.sayHello(delay)
            elsif c == 'd'
                if secure
                    puts "secure datagrams are not supported"
                else
                    datagram.sayHello(delay)
                end
            elsif c == 'D'
                if secure
                    puts "secure datagrams are not supported"
                else
                    batchDatagram.sayHello(delay)
                end
            elsif c == 'f'
                batchOneway.ice_flushBatchRequests()
                batchDatagram.ice_flushBatchRequests()
            elsif c == 'T'
                if timeout == -1
                    timeout = 2000
                else
                    timeout = -1
                end

                twoway = twoway.ice_invocationTimeout(timeout)
                oneway = oneway.ice_invocationTimeout(timeout)
                batchOneway = batchOneway.ice_invocationTimeout(timeout)

                if timeout == -1
                    puts "timeout is now switched off"
                else
                    puts "timeout is now set to 2000ms"
                end
            elsif c == 'P'
                if delay == 0
                    delay = 2500
                else
                    delay = 0
                end

                if delay == 0
                    puts "server delay is now deactivated"
                else
                    puts "server delay is now set to 2500ms"
                end
            elsif c == 'S'
                secure = !secure

                twoway = twoway.ice_secure(secure)
                oneway = oneway.ice_secure(secure)
                batchOneway = batchOneway.ice_secure(secure)
                datagram = datagram.ice_secure(secure)
                batchDatagram = batchDatagram.ice_secure(secure)

                if secure
                    puts "secure mode is now on"
                else
                    puts "secure mode is now off"
                end
            elsif c == 's'
                twoway.shutdown()
            elsif c == 'x'
                # Nothing to do
            elsif c == '?'
                menu()
            else
                puts "unknown command `" + c + "'"
                menu()
            end
        rescue Ice::Exception => ex
            puts ex
        rescue EOFError
            return 1
        end
    end

    return 0
end

def menu
    print <<MENU
usage:
t: send greeting as twoway
o: send greeting as oneway
O: send greeting as batch oneway
d: send greeting as datagram
D: send greeting as batch datagram
f: flush all batch requests
T: set a timeout
P: set a server delay
S: switch secure mode on/off
s: shutdown server
x: exit
?: help
MENU
end

status = Ice::initialize(ARGV, "config.client") { |communicator, args| run(communicator, args) }
exit(status)
