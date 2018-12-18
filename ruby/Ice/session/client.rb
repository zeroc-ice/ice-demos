#!/usr/bin/env ruby
# **********************************************************************
#
# Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
#
# **********************************************************************

require 'Ice'
require 'thread'
require 'timeout'

Ice::loadSlice('Session.ice')

def run(communicator, args)
    if args.length > 0
        puts $0 + ": too many arguments"
        return 1
    end

    while true
        print "Please enter your name ==> "
        STDOUT.flush
        name = STDIN.readline.chomp
        if name.length > 0
            break
        end
    end

    base = communicator.propertyToProxy('SessionFactory.Proxy')
    factory = Demo::SessionFactoryPrx::checkedCast(base)
    if not factory
        puts $0 + ": invalid proxy"
        return 1
    end

    session = factory.create(name)

    hellos = []

    menu()

    destroy = true
    shutdown = false
    while true
        begin
            print "==> "
            STDOUT.flush
            c = STDIN.readline.chomp
            if c =~ /^[0-9]+$/
                index = c.to_i
                if index < hellos.length
                    hello = hellos[index]
                    hello.sayHello()
                else
                    puts "Index is too high. " + hellos.length.to_s + " hello objects exist so far.\n" +\
                    "Use `c' to create a new hello object."
                end
            elsif c == 'c'
                hellos.push(session.createHello())
                puts "Created hello object " + (hellos.length - 1).to_s
            elsif c == 's'
                destroy = false
                shutdown = true
                break
            elsif c == 'x'
                break
            elsif c == 't'
                destroy = false
                break
            elsif c == '?'
                menu()
            else
                puts "unknown command `" + c + "'"
                menu()
            end
        rescue EOFError
            break
        end
    end

    if destroy
        session.destroy
    end

    if shutdown
        factory.shutdown()
    end

    return 0
end

def menu
    print <<MENU
usage:
c:     create a new per-client hello object
0-9:   send a greeting to a hello object
s:     shutdown the server and exit
x:     exit
t:     exit without destroying the session
?:     help
MENU
end

status = Ice::initialize(ARGV, "config.client") { |communicator, args| run(communicator, args) }
exit(status)
