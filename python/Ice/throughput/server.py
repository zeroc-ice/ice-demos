#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import signal
import sys
import Ice

Ice.loadSlice('Throughput.ice')
import Demo


class ThroughputI(Demo.Throughput):
    def __init__(self):
        self.warmup = False

        if sys.version_info[0] == 2:
            b = []
            b[0:Demo.ByteSeqSize] = range(0, Demo.ByteSeqSize)
            b = ['\x00' for x in b]
            self.byteSeq = ''.join(b)
        else:
            self.byteSeq = bytes([0 for x in range(0, Demo.ByteSeqSize)])

        self.stringSeq = []
        self.stringSeq[0:Demo.StringSeqSize] = range(0, Demo.StringSeqSize)
        self.stringSeq = ["hello" for x in self.stringSeq]

        self.structSeq = []
        self.structSeq[0:Demo.StringDoubleSeqSize] = range(0, Demo.StringDoubleSeqSize)
        for i in range(0, Demo.StringDoubleSeqSize):
            self.structSeq[i] = Demo.StringDouble()
            self.structSeq[i].s = "hello"
            self.structSeq[i].d = 3.14

        self.fixedSeq = []
        self.fixedSeq[0:Demo.FixedSeqSize] = range(0, Demo.FixedSeqSize)
        for i in range(0, Demo.FixedSeqSize):
            self.fixedSeq[i] = Demo.Fixed()
            self.fixedSeq[i].i = 0
            self.fixedSeq[i].j = 0
            self.fixedSeq[i].d = 0.0

    def needsWarmup(self, current):
        self.warmup = False
        return False

    def startWarmup(self, current):
        self.warmup = True

    def endWarmup(self, current):
        self.warmup = False

    def sendByteSeq(self, seq, current):
        pass

    def recvByteSeq(self, current):
        if self.warmup:
            return ''.join(['\x00'])
        else:
            return self.byteSeq

    def echoByteSeq(self, seq, current):
        return seq

    def sendStringSeq(self, seq, current):
        pass

    def recvStringSeq(self, current):
        if self.warmup:
            return [""]
        else:
            return self.stringSeq

    def echoStringSeq(self, seq, current):
        return seq

    def sendStructSeq(self, seq, current):
        pass

    def recvStructSeq(self, current):
        if self.warmup:
            return [Demo.StringDouble()]
        else:
            return self.structSeq

    def echoStructSeq(self, seq, current):
        return seq

    def sendFixedSeq(self, seq, current):
        pass

    def recvFixedSeq(self, current):
        if self.warmup:
            return [Demo.Fixed()]
        else:
            return self.fixedSeq

    def echoFixedSeq(self, seq, current):
        return seq

    def shutdown(self, current):
        current.adapter.getCommunicator().shutdown()


#
# Ice.initialize returns an initialized Ice communicator,
# the communicator is destroyed once it goes out of scope.
#
with Ice.initialize(sys.argv, "config.server") as communicator:

    #
    # Install a signal handler to shutdown the communicator on Ctrl-C
    #
    signal.signal(signal.SIGINT, lambda signum, handler: communicator.shutdown())

    #
    # The communicator initialization removes all Ice-related arguments from argv
    #
    if len(sys.argv) > 1:
        print(sys.argv[0] + ": too many arguments")
        sys.exit(1)

    adapter = communicator.createObjectAdapter("Throughput")
    adapter.add(ThroughputI(), Ice.stringToIdentity("throughput"))
    adapter.activate()
    communicator.waitForShutdown()
