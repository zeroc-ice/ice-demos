#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import signal, sys, Ice, IceStorm, getopt

Ice.loadSlice('Clock.ice')
import Demo

class ClockI(Demo.Clock):
    def tick(self, date, current):
        print(date)

def usage():
    print("Usage: " + sys.argv[0] +
          " [--batch] [--datagram|--twoway|--ordered|--oneway] [--retryCount count] [--id id] [topic]")

def run(communicator, args):
    try:
        opts, args = getopt.getopt(args[1:], '', ['datagram', 'twoway', 'oneway', 'ordered', 'batch',
                                                  'retryCount=', 'id='])
    except getopt.GetoptError:
        usage()
        return 1

    batch = False
    option = "None"
    topicName = "time"
    id = ""
    retryCount = ""

    for o, a in opts:
        oldoption = option
        if o == "--datagram":
            option = "Datagram"
        elif o == "--twoway":
            option = "Twoway"
        elif o == "--ordered":
            option = "Ordered"
        elif o == "--oneway":
            option = "Oneway"
        elif o == "--batch":
            batch = True
        elif o == "--id":
            id = a
        elif o == "--retryCount":
            retryCount = a
        if oldoption != option and oldoption != "None":
            usage()
            return 1

    if len(args) > 1:
        usage()
        return 1

    if len(args) > 0:
        topicName = args[0]

    if len(retryCount) > 0:
        if option == "None":
            option = "Twoway"
        elif option != "Twoway" and option != "Ordered":
            print(sys.argv[0] + ": retryCount requires a twoway proxy")
            return 1

    if batch and (option in ("Twoway", "Ordered")):
        print(sys.argv[0] + ": batch can only be set with oneway or datagram")
        return 1

    manager = IceStorm.TopicManagerPrx.checkedCast(communicator.propertyToProxy('TopicManager.Proxy'))
    if not manager:
        print(args[0] + ": invalid proxy")
        return 1

    #
    # Retrieve the topic.
    #
    try:
        topic = manager.retrieve(topicName)
    except IceStorm.NoSuchTopic as e:
        try:
            topic = manager.create(topicName)
        except IceStorm.TopicExists as ex:
            print(sys.argv[0] + ": temporary error. try again")
            return 1

    adapter = communicator.createObjectAdapter("Clock.Subscriber")

    #
    # Add a servant for the Ice object. If --id is used the identity
    # comes from the command line, otherwise a UUID is used.
    #
    # id is not directly altered since it is used below to detect
    # whether subscribeAndGetPublisher can raise AlreadySubscribed.
    #

    subId = Ice.Identity()
    subId.name = id
    if len(subId.name) == 0:
        subId.name = Ice.generateUUID()
    subscriber = adapter.add(ClockI(), subId)

    #
    # Activate the object adapter before subscribing.
    #
    adapter.activate()

    qos = {}
    if len(retryCount) > 0:
        qos["retryCount"] = retryCount

    #
    # Set up the proxy.
    #
    if option == "Datagram":
        if batch:
            subscriber = subscriber.ice_batchDatagram()
        else:
            subscriber = subscriber.ice_datagram()
    elif option == "Twoway":
        # Do nothing to the subscriber proxy. Its already twoway.
        pass
    elif option == "Ordered":
        # Do nothing to the subscriber proxy. Its already twoway.
        qos["reliability"] = "ordered"
    elif option == "Oneway" or option == "None":
        if batch:
            subscriber = subscriber.ice_batchOneway()
        else:
            subscriber = subscriber.ice_oneway()

    try:
        topic.subscribeAndGetPublisher(qos, subscriber)
    except IceStorm.AlreadySubscribed as ex:
        # If we're manually setting the subscriber id ignore.
        if len(id) == 0:
            raise
        print("reactivating persistent subscriber")

    signal.signal(signal.SIGINT, lambda signum, frame: communicator.shutdown())
    communicator.waitForShutdown()

    #
    # Unsubscribe all subscribed objects.
    #
    topic.unsubscribe(subscriber)

    return 0

status = 0

#
# Ice.initialize returns an initialized Ice communicator,
# the communicator is destroyed once it goes out of scope.
#
with Ice.initialize(sys.argv, "config.sub") as communicator:
    status = run(communicator, sys.argv)
sys.exit(status)
