//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Ice
import IceStorm
import Foundation

func usage() {
    print("Usage: [--datagram|--twoway|--oneway] [topic]")
}

func run() -> Int32 {
    do {
        var args = [String](CommandLine.arguments.dropFirst())
        let communicator = try Ice.initialize(args: &args, configFile: "config.pub")
        defer {
            communicator.destroy()
        }

        var option = "None"
        var topicName = "time"

        for var i in 0..<args.count {
            let oldoption = option

            if args[i] == "--datagram" {
                option = "Datagram"
            } else if args[i] == "--twoway" {
                option = "Twoway"
            } else if args[i] == "--oneway" {
                option = "Oneway"
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

        guard let base = try communicator.propertyToProxy("TopicManager.Proxy"),
              let manager = try checkedCast(prx: base, type: IceStorm.TopicManagerPrx.self) else {
                print("invalid proxy")
                return 1
        }

        //
        // Retrieve the topic.
        //
        var topic: IceStorm.TopicPrx!
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

        //
        // Get the topic's publisher object, and create a Clock proxy with
        // the mode specified as an argument of this application.
        //
        guard var publisher = try topic.getPublisher() else {
            print("Error getting publisher proxy")
            return 1
        }
        if option == "Datagram" {
            publisher = publisher.ice_datagram()
        } else if option == "Twoway" {
            // Do nothing.
        } else { // if(oneway)
            publisher = publisher.ice_oneway()
        }

        let clock = uncheckedCast(prx: publisher, type: ClockPrx.self)

        print("publishing tick events. Press ^C to terminate the application.")

        let dateFormatter = DateFormatter()
        dateFormatter.dateStyle = .short
        dateFormatter.timeStyle = .medium

        do {
            while true {
                try clock.tick(dateFormatter.string(from: Date()))
                Thread.sleep(forTimeInterval: TimeInterval(1))
            }
        } catch is Ice.CommunicatorDestroyedException {} // Ignore

        return 0
    } catch {
        print("Error: \(error)\n")
        return 1
    }
}

exit(run())
