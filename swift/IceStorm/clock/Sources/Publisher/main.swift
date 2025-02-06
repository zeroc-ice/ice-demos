// Copyright (c) ZeroC, Inc.

import Foundation
import Ice
import IceStorm

// Automatically flush stdout
setbuf(__stdoutp, nil)

func usage() {
    print("Usage: [--datagram|--twoway|--oneway] [topic]")
}

enum Option: String {
    case none = ""
    case datagram = "--datagram"
    case twoway = "--twoway"
    case oneway = "--oneway"
}

// CtrlCHandler is a helper class that handles Ctrl+C and similar signals. It must be constructed at the beginning
// of the program, before creating an Ice communicator or starting any thread.
let ctrlCHandler = Ice.CtrlCHandler()

// Create an Ice communicator to initialize the Ice runtime.
var args = [String](CommandLine.arguments.dropFirst())
let communicator = try Ice.initialize(args: &args, configFile: "config.pub")
defer {
    communicator.destroy()
}

// Parse command-line options.

var option: Option = .none
var topicName = "time"

for var i in 0..<args.count {
    let oldOption = option

    if args[i].starts(with: "--") {
        guard let o = Option(rawValue: args[i]) else {
            usage()
            exit(1)
        }
        option = o
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

// Create the topic manager proxy.
let manager = try uncheckedCast(
    prx: communicator.propertyToProxy("TopicManager.Proxy")!,
    type: IceStorm.TopicManagerPrx.self)

// Retrieve the topic.
var topic: IceStorm.TopicPrx!
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

// Get the topic's publisher object, and create a Clock proxy with the mode specified as an argument of this
// application.
guard var publisher = try await topic.getPublisher() else {
    print("Error getting publisher proxy")
    exit(1)
}

switch option {
case .datagram:
    publisher = publisher.ice_datagram()
case .twoway:
    // Do Nothing
    break
default:
    publisher = publisher.ice_oneway()
}

// Downcast the proxy to the Clock type using uncheckedCast.
let clock = uncheckedCast(prx: publisher, type: ClockPrx.self)

print("publishing tick events. Press ^C to terminate the application.")

let dateFormatter = DateFormatter()
dateFormatter.dateFormat = "dd/MM/YYYY HH:mm:ss"

// Send a tick every second until cancelled in a background task.
let task = Task {
    while true {
        do {
            try await clock.tick(dateFormatter.string(from: Date()))
        } catch let error as Ice.LocalException {
            print("tick invocation failed with: \(error.message)")
        }
        try await Task.sleep(for: .seconds(1))
    }
}

// Wait until the user presses Ctrl+C.
let signal = await ctrlCHandler.catchSignal()
print("Caught signal \(signal), exiting...")

// Cancel the background task and wait for its completion.
task.cancel()
do {
    try await task.value
} catch is CancellationError {
    // expected
}
