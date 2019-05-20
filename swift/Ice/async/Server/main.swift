//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Foundation
import Ice
import PromiseKit

class HelloI: Hello {
    var workQueue: OperationQueue
    var serialQueue: DispatchQueue // Used for blocking the change of workQueue.isSuspended
    var concurrentQueue: DispatchQueue // DispatchQueue used in workQueue

    init(_ queue: OperationQueue) {
        workQueue = queue
        serialQueue = DispatchQueue(label: "com.zeroc.demo.ice.async.server.serialqueue")
        concurrentQueue = DispatchQueue(label: "com.zeroc.demo.ice.async.server.workqueue",
                                        qos: .default,
                                        attributes: .concurrent)
        workQueue.underlyingQueue = concurrentQueue
    }

    func sayHelloAsync(delay: Int32, current _: Current) -> Promise<Void> {
        return Promise { seal in
            if delay == 0 {
                print("Hello World!")
                seal.fulfill(())
            } else {
                serialQueue.sync {
                    guard self.workQueue.isSuspended == false else {
                        seal.reject(RequestCanceledException())
                        return
                    }
                    self.workQueue.addOperation {
                        Thread.sleep(forTimeInterval: TimeInterval(delay) / 1000)
                        print("Belated Hello World!")
                        seal.fulfill(())
                    }
                }
            }
        }
    }

    func shutdown(current: Current) throws {
        print("Shutting down...")
        // Setting this property to true prevents the queue from starting any queued operations,
        // but already executing operations continue to execute.
        serialQueue.sync {
            workQueue.isSuspended = true
        }
        if let adapter = current.adapter {
            adapter.getCommunicator().shutdown()
        }
    }
}

func run() -> Int32 {
    do {
        var args = CommandLine.arguments
        let workQueue = OperationQueue()
        let communicator = try Ice.initialize(args: &args, configFile: "config.server")

        defer {
            communicator.destroy()
        }
        guard args.count == 1 else {
            print("too many arguments\n")
            return 1
        }

        let adapter = try communicator.createObjectAdapter("Hello")
        try adapter.add(servant: HelloI(workQueue), id: Ice.stringToIdentity("hello"))
        try adapter.activate()

        communicator.waitForShutdown()
        workQueue.waitUntilAllOperationsAreFinished()
    } catch {
        print("Error: \(error)\n")
        return 1
    }
    return 0
}

exit(run())
