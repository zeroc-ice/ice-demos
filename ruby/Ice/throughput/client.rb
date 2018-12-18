#!/usr/bin/env ruby
# **********************************************************************
#
# Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
#
# **********************************************************************

require 'Ice'

Ice::loadSlice('Throughput.ice')

def run(communicator, args)
    if args.length > 0
        puts $0 + ": too many arguments"
        return 1
    end

    throughput = Demo::ThroughputPrx::checkedCast(communicator.propertyToProxy('Throughput.Proxy'))
    if not throughput
        puts $0 + ": invalid proxy"
        return 1
    end
    throughputOneway = Demo::ThroughputPrx::uncheckedCast(throughput.ice_oneway())

    byteSeq = "\0" * Demo::ByteSeqSize

    stringSeq = Array.new(Demo::StringSeqSize, "hello")

    stringDouble = Demo::StringDouble.new
    stringDouble.s = "hello"
    stringDouble.d = 3.14
    structSeq = Array.new(Demo::StringDoubleSeqSize, stringDouble)

    fixed = Demo::Fixed.new
    fixed.i = 0
    fixed.j = 0
    fixed.d = 0.0
    fixedSeq = Array.new(Demo::FixedSeqSize, fixed)

    #
    # To allow cross-language tests we may need to "warm up" the
    # server. The warm up is to ensure that any JIT compiler will
    # have converted any hotspots to native code. This ensures an
    # accurate throughput measurement.
    #
    if throughput.needsWarmup()
        throughput.startWarmup()

        warmupBytes = "\x00"
        warmupStrings = Array.new(1, "hello")
        warmupStructs = Array.new(1, Demo::StringDouble.new)
        warmupFixed = Array.new(1, Demo::Fixed.new)

        print "warming up the server..."
        STDOUT.flush
        for i in (0..10000)
            throughput.sendByteSeq(warmupBytes)
            throughput.sendStringSeq(warmupStrings)
            throughput.sendStructSeq(warmupStructs)
            throughput.sendFixedSeq(warmupFixed)

            throughput.recvByteSeq()
            throughput.recvStringSeq()
            throughput.recvStructSeq()
            throughput.recvFixedSeq()

            throughput.echoByteSeq(warmupBytes)
            throughput.echoStringSeq(warmupStrings)
            throughput.echoStructSeq(warmupStructs)
            throughput.echoFixedSeq(warmupFixed)
        end

        throughput.endWarmup()

        print " ok\n"
    else
        throughput.ice_ping() # Initial ping to setup the connection.
    end

    menu()

    currentType = '1'
    seqSize = Demo::ByteSeqSize

    c = nil
    while c != 'x'
        begin
            print "==> "
            STDOUT.flush
            c = STDIN.readline.chomp

            repetitions = 100

            if c == '1' or c == '2' or c == '3' or c == '4'
                currentType = c
                if c == '1'
                    puts "using byte sequences"
                    seqSize = Demo::ByteSeqSize
                elsif c == '2'
                    puts "using string sequences"
                    seqSize = Demo::StringSeqSize
                elsif c == '3'
                    puts "using variable-length struct sequences"
                    seqSize = Demo::StringDoubleSeqSize
                elsif c == '4'
                    puts "using fixed-length struct sequences"
                    seqSize = Demo::FixedSeqSize
                end
            elsif c == 't' or c == 'o' or c == 'r' or c == 'e'

                if currentType == '1'
                    repetitions = 1000 # Use more iterations for  byte sequences as it's a lot faster
                end

                if c == 't' or c == 'o'
                    print "sending "
                elsif c == 'r'
                    print "receiving "
                elsif c == 'e'
                    print "sending and receiving "
                end

                print repetitions.to_s + " "
                if currentType == '1'
                    print "byte "
                elsif currentType == '2'
                    print "string "
                elsif currentType == '3'
                    print "variable-length struct "
                elsif currentType == '4'
                    print "fixed-length struct "
                end

                if c == 'o'
                    printf "sequences of size %d as oneway...\n", seqSize
                else
                    printf "sequences of size %d...\n", seqSize
                end

                tsec = Time.now

                for i in (0..repetitions)
                    if currentType == '1'
                        if c == 't'
                            throughput.sendByteSeq(byteSeq)
                        elsif c == 'o'
                            throughputOneway.sendByteSeq(byteSeq)
                        elsif c == 'r'
                            throughput.recvByteSeq()
                        elsif c == 'e'
                            throughput.echoByteSeq(byteSeq)
                        end
                    elsif currentType == '2'
                        if c == 't'
                            throughput.sendStringSeq(stringSeq)
                        elsif c == 'o'
                            throughputOneway.sendStringSeq(stringSeq)
                        elsif c == 'r'
                            throughput.recvStringSeq()
                        elsif c == 'e'
                            throughput.echoStringSeq(stringSeq)
                        end
                    elsif currentType == '3'
                        if c == 't'
                            throughput.sendStructSeq(structSeq)
                        elsif c == 'o'
                            throughputOneway.sendStructSeq(structSeq)
                        elsif c == 'r'
                            throughput.recvStructSeq()
                        elsif c == 'e'
                            throughput.echoStructSeq(structSeq)
                        end
                    elsif currentType == '4'
                        if c == 't'
                            throughput.sendFixedSeq(fixedSeq)
                        elsif c == 'o'
                            throughputOneway.sendFixedSeq(fixedSeq)
                        elsif c == 'r'
                            throughput.recvFixedSeq()
                        elsif c == 'e'
                            throughput.echoFixedSeq(fixedSeq)
                        end
                    end
                end

                tsec = Time.now - tsec
                tmsec = tsec * 1000.0
                printf "time for %d sequences: %.3fms\n", repetitions, tmsec
                printf "time per sequence: %.3fms\n", tmsec / repetitions
                wireSize = 0
                if currentType == '1'
                    wireSize = 1
                elsif currentType == '2'
                    wireSize = stringSeq[0].length
                elsif currentType == '3'
                    wireSize = structSeq[0].s.length
                    wireSize += 8
                elsif currentType == '4'
                    wireSize = 16
                end
                mbit = repetitions * seqSize * wireSize * 8.0 / tsec / 1000000.0
                if c == 'e'
                    mbit = mbit * 2
                end
                printf "throughput: %.3fMbps\n", mbit
            elsif c == 's'
                throughput.shutdown()
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

toggle type of data to send:
1: sequence of bytes (default)
2: sequence of strings (\"hello\")
3: sequence of structs with a string (\"hello\") and a double
4: sequence of structs with two ints and a double

select test to run:
t: Send sequence as twoway
o: Send sequence as oneway
r: Receive sequence
e: Echo (send and receive) sequence

other commands:
s: shutdown server
x: exit
?: help
MENU
end

status = Ice::initialize(ARGV, "config.client") { |communicator, args| run(communicator, args) }
exit(status)
