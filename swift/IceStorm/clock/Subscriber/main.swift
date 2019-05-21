//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Ice
import IceStorm
import Foundation

func usage() {
    print("Usage: [--batch] [--datagram|--twoway|--ordered|--oneway] [--retryCount count] [--id id] [topic]")
}

class ClockI: Clock {
    func tick(time date: String, current: Ice.Current) throws {
        print("DATE: \(date)")
    }
}

func run() -> Int32 {
    do {
        var args = [String](CommandLine.arguments.dropFirst())
        let communicator = try Ice.initialize(args: &args, configFile: "config.sub")
        defer {
            communicator.destroy()
        }
        args = try communicator.getProperties().parseCommandLineOptions(prefix: "Clock", options: args)

        var topicName = "time"
        var option = "None"
        var batch = false
        var id: String? = nil
        var retryCount: String? = nil

        for var i in 0..<args.count {
            let oldoption = option
            if args[i] == "--datagram" {
                option = "Datagram"
            } else if args[i] == "--twoway" {
                option = "Twoway"
            } else if args[i] == "--ordered" {
                option = "Ordered"
            } else if args[i] == "--oneway" {
                option = "Oneway"
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

            if oldoption != option && oldoption != "None" {
                usage()
                return 1
            }
        }

        if retryCount != nil {
            if option == "None" {
                option = "Twoway";
            } else if option != "Twoway" && option != "Ordered" {
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

        var subscriber = try adapter.add(servant: ClockI(), id: subId)

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
        if option == "Datagram" {
            subscriber = batch ? subscriber.ice_batchDatagram() : subscriber.ice_datagram()
        } else if option == "Twoway" {
            // Do nothing to the subscriber proxy. Its already twoway.
        } else if option == "Ordered" {
            // Do nothing to the subscriber proxy. Its already twoway.
            qos["reliability"] = "ordered"
        } else if option == "Oneway" || option == "None" {
            subscriber = batch ? subscriber.ice_batchOneway() : subscriber.ice_oneway()
        }

        do {
            _ = try topic.subscribeAndGetPublisher(theQoS: qos, subscriber: subscriber)
        } catch let ex as IceStorm.AlreadySubscribed {
            // If we're manually setting the subscriber id ignore.
            if id == nil {
                throw ex
            }
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
