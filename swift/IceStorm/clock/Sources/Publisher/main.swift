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

func run(ctrlCHandler: Ice.CtrlCHandler) async -> Int32 {
    do {
        var args = [String](CommandLine.arguments.dropFirst())

        let communicator = try Ice.initialize(args: &args, configFile: "config.pub")
        defer {
            communicator.destroy()
        }

        var option: Option = .none
        var topicName = "time"

        for var i in 0 ..< args.count {
            let oldoption = option

            if args[i].starts(with: "--") {
                guard let o = Option(rawValue: args[i]) else {
                    usage()
                    return 1
                }
                option = o
            } else {
                topicName = args[i]
                i += 1
                break
            }

            if oldoption != option, oldoption != .none {
                usage()
                return 1
            }
        }

        guard let base = try communicator.propertyToProxy("TopicManager.Proxy"),
            let manager = try await checkedCast(prx: base, type: IceStorm.TopicManagerPrx.self) else {
            print("invalid proxy")
            return 1
        }

        //
        // Retrieve the topic.
        //
        var topic: IceStorm.TopicPrx!
        do {
            topic = try await manager.retrieve(topicName)
        } catch is IceStorm.NoSuchTopic {
            do {
                topic = try await manager.create(topicName)
            } catch is IceStorm.TopicExists {
                print("temporary error. try again.")
                return 1
            }
        }

        //
        // Get the topic's publisher object, and create a Clock proxy with
        // the mode specified as an argument of this application.
        //
        guard var publisher = try await topic.getPublisher() else {
            print("Error getting publisher proxy")
            return 1
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

        let clock = uncheckedCast(prx: publisher, type: ClockPrx.self)

        print("publishing tick events. Press ^C to terminate the application.")

        let dateFormatter = DateFormatter()
        dateFormatter.dateFormat = "dd/MM/YYYY HH:mm:ss"

        // Send a tick every second until cancelled
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

        let signal = await ctrlCHandler.catchSignal()
        print("Caught signal \(signal), exiting...")

        task.cancel()
        do {
            try await task.value
        } catch is CancellationError {
            // expected
        }

        return 0
    } catch {
        print("Error: \(error)\n")
        return 1
    }
}

let ctrlCHandler = Ice.CtrlCHandler()
exit(await run(ctrlCHandler: ctrlCHandler))
