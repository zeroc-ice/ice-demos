// Copyright (c) ZeroC, Inc.

import Foundation
import Ice
import IceStorm

// Automatically flush stdout
setbuf(__stdoutp, nil)

func usage() {
    print(
        "Usage: [--batch] [--datagram|--twoway|--ordered|--oneway] [--retryCount count] [--id id] [topic]"
    )
}

/// ClockI is an Ice servant that implements Slice interface Clock.
class ClockI: Clock {
    func tick(time: String, current _: Ice.Current) throws {
        print(time)
    }
}

enum Option: String {
    case none = ""
    case datagram = "--datagram"
    case twoway = "--twoway"
    case ordered = "--ordered"
    case oneway = "--oneway"
}

// CtrlCHandler is a helper class that handles Ctrl+C and similar signals. It must be constructed at the beginning
// of the program, before creating an Ice communicator or starting any thread.
let ctrlCHandler = CtrlCHandler()

// Create an Ice communicator to initialize the Ice runtime.
var args = [String](CommandLine.arguments.dropFirst())
let communicator = try Ice.initialize(args: &args, configFile: "config.sub")
defer {
    communicator.destroy()
}

// Parse command-line options.

args = try communicator.getProperties().parseCommandLineOptions(prefix: "Clock", options: args)

var topicName = "time"
var option: Option = .none
var batch = false
var id: String?
var retryCount: String?

for var i in 0..<args.count {
    let oldOption = option
    if let o = Option(rawValue: args[i]) {
        option = o
    } else if args[i] == "--batch" {
        batch = true
    } else if args[i] == "--id" {
        i += 1
        if i >= args.count {
            usage()
            exit(1)
        }
        id = args[i]
    } else if args[i] == "--retryCount" {
        i += 1
        if i >= args.count {
            usage()
            exit(1)
        }
        retryCount = args[i]
    } else if args[i].starts(with: "--") {
        usage()
        exit(1)
    } else {
        topicName = args[i]
        i += 1
        break
    }

    if oldOption != option, oldOption != .none {
        usage()
        exit(1)
    }
}

if retryCount != nil {
    if option == .none {
        option = .twoway
    } else if option != .twoway, option != .ordered {
        usage()
        exit(1)
    }
}

// Create the topic manager proxy.
let manager = try uncheckedCast(
    prx: communicator.propertyToProxy("TopicManager.Proxy")!,
    type: IceStorm.TopicManagerPrx.self)

// Retrieve the topic from IceStorm.
let topic: IceStorm.TopicPrx!
do {
    topic = try await manager.retrieve(topicName)
} catch is IceStorm.NoSuchTopic {
    do {
        topic = try await manager.create(topicName)
    } catch is IceStorm.TopicExists {
        print("temporary error. try again.")
        exit(1)
    }
}

// Create the object adapter that hosts the subscriber servant.
let adapter = try communicator.createObjectAdapter("Clock.Subscriber")

// Add a servant to the object adapter. If --id is used the identity comes from the command line, otherwise a UUID is
// used. id is not directly altered since it is used below to detect whether subscribeAndGetPublisher can throw
// AlreadySubscribed.
let subId = Ice.Identity(name: id ?? UUID().uuidString, category: "")
var subscriber = try adapter.add(servant: ClockI(), id: subId)

// Activate the object adapter before registering the subscriber with the IceStorm topic.
try adapter.activate()

var qos: [String: String] = [:]
if let retryCount = retryCount {
    qos["retryCount"] = retryCount
}

// Set up the subscriber proxy.
switch option {
case .datagram:
    subscriber = batch ? subscriber.ice_batchDatagram() : subscriber.ice_datagram()
case .ordered:
    // Do nothing to the subscriber proxy. Its already twoway.
    qos["reliability"] = "ordered"
case .oneway,
    .none:
    subscriber = batch ? subscriber.ice_batchOneway() : subscriber.ice_oneway()
case .twoway:
    // Do nothing to the subscriber proxy. Its already twoway.
    break
}

do {
    // Register the subscriber with the IceStorm topic.
    _ = try await topic.subscribeAndGetPublisher(theQoS: qos, subscriber: subscriber)
} catch is IceStorm.AlreadySubscribed {
    // This should never occur when subscribing with a UUID.
    precondition(id != nil)
    print("reactivating persistent subscriber")
}

// Shutdown the communicator when the user presses Ctrl+C.
ctrlCHandler.setCallback { signal in
    print("Caught signal \(signal), shutting down...")
    communicator.shutdown()
}

// Wait until the communicator is shut down. Here, this occurs when the user presses Ctrl+C.
await communicator.shutdownCompleted()

// Unsubscribe from the topic in IceStorm.
try await topic.unsubscribe(subscriber)
