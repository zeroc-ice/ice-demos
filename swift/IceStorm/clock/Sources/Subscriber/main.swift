//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Foundation
import Ice
import IceStorm

// Automatically flush stdout
setbuf(__stdoutp, nil)

func usage() {
    print("Usage: [--batch] [--datagram|--twoway|--ordered|--oneway] [--retryCount count] [--id id] [topic]")
}

class ClockI: Clock {
    func tick(time date: String, current _: Ice.Current) throws {
        print(date)
    }
}

enum Option: String {
    case none = ""
    case datagram = "--datagram"
    case twoway = "--twoway"
    case ordered = "--ordered"
    case oneway = "--oneway"
}

func run() -> Int32 {
    do {
        var args = [String](CommandLine.arguments.dropFirst())
        signal(SIGTERM, SIG_IGN)
        signal(SIGINT, SIG_IGN)

        let communicator = try Ice.initialize(args: &args, configFile: "config.sub")
        defer {
            communicator.destroy()
        }

        let sigintSource = DispatchSource.makeSignalSource(signal: SIGINT,
                                                           queue: DispatchQueue.global())
        let sigtermSource = DispatchSource.makeSignalSource(signal: SIGTERM,
                                                            queue: DispatchQueue.global())
        sigintSource.setEventHandler { communicator.shutdown() }
        sigtermSource.setEventHandler { communicator.shutdown() }
        sigintSource.resume()
        sigtermSource.resume()

        args = try communicator.getProperties().parseCommandLineOptions(prefix: "Clock", options: args)

        var topicName = "time"
        var option: Option = .none
        var batch = false
        var id: String?
        var retryCount: String?

        for var i in 0 ..< args.count {
            let oldoption = option
            if let o = Option(rawValue: args[i]) {
                option = o
            } else if args[i] == "--batch" {
                batch = true
            } else if args[i] == "--id" {
                i += 1
                if i >= args.count {
                    usage()
                    return 1
                }
                id = args[i]
            } else if args[i] == "--retryCount" {
                i += 1
                if i >= args.count {
                    usage()
                    return 1
                }
                retryCount = args[i]
            } else if args[i].starts(with: "--") {
                usage()
                return 1
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

        if retryCount != nil {
            if option == .none {
                option = .twoway
            } else if option != .twoway, option != .ordered {
                usage()
                return 1
            }
        }

        guard let base = try communicator.propertyToProxy("TopicManager.Proxy"),
            let manager = try checkedCast(prx: base, type: IceStorm.TopicManagerPrx.self) else {
            print("invalid proxy")
            return 1
        }

        //
        // Retrieve the topic.
        //
        let topic: IceStorm.TopicPrx!
        do {
            topic = try manager.retrieve(topicName)
        } catch is IceStorm.NoSuchTopic {
            do {
                topic = try manager.create(topicName)
            } catch is IceStorm.TopicExists {
                print("temporary error. try again.")
                return 1
            }
        }

        let adapter = try communicator.createObjectAdapter("Clock.Subscriber")
        //
        // Add a servant for the Ice object. If --id is used the
        // identity comes from the command line, otherwise a UUID is
        // used.
        //
        // id is not directly altered since it is used below to
        // detect whether subscribeAndGetPublisher can raise
        // AlreadySubscribed.
        //
        let subId = Ice.Identity(name: id ?? UUID().uuidString, category: "")

        var subscriber = try adapter.add(servant: ClockDisp(ClockI()), id: subId)

        //
        // Activate the object adapter before subscribing.
        //
        try adapter.activate()

        var qos: [String: String] = [:]

        if let retryCount = retryCount {
            qos["retryCount"] = retryCount
        }

        //
        // Set up the proxy.
        //
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
            _ = try topic.subscribeAndGetPublisher(theQoS: qos, subscriber: subscriber)
        } catch is IceStorm.AlreadySubscribed {
            // Must never happen when subscribing with an UUID
            precondition(id != nil)
            print("reactivating persistent subscriber")
        }
        communicator.waitForShutdown()
        try topic.unsubscribe(subscriber)

        return 0
    } catch {
        print("Error: \(error)\n")
        return 1
    }
}

exit(run())
