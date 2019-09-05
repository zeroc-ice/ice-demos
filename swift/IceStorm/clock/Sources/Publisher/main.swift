//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Foundation
import PromiseKit
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

func run() -> Int32 {
    do {
        var args = [String](CommandLine.arguments.dropFirst())
        signal(SIGTERM, SIG_IGN)
        signal(SIGINT, SIG_IGN)

        let communicator = try Ice.initialize(args: &args, configFile: "config.pub")
        defer {
            communicator.destroy()
        }

        let sigintSource = DispatchSource.makeSignalSource(signal: SIGINT, queue: DispatchQueue.main)
        sigintSource.setEventHandler {
            communicator.destroy()
        }
        sigintSource.resume()

        let sigtermSource = DispatchSource.makeSignalSource(signal: SIGTERM, queue: DispatchQueue.main)
        sigtermSource.setEventHandler {
            communicator.destroy()
        }
        sigtermSource.resume()

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

        let t = DispatchSource.makeTimerSource()
        t.schedule(deadline: .now(), repeating: .seconds(1))
        t.setEventHandler {
            do {
                try clock.tick(dateFormatter.string(from: Date()))
            } catch is CommunicatorDestroyedException {
                t.suspend()
                exit(0)
            } catch {
                t.suspend()
                print("Error: \(error)\n")
                communicator.destroy()
                exit(1)
            }
        }
        t.activate()
        Dispatch.dispatchMain()
    } catch {
        print("Error: \(error)\n")
        return 1
    }
}

exit(run())
