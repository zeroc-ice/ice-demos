//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Foundation
import Ice
import PromiseKit

class HelloI: Hello {

    var done: Bool = false
    var doneSemaphore = DispatchSemaphore(value: 1)

    var workQueue: OperationQueue
    var concurrentQueue: DispatchQueue // DispatchQueue used in workQueue

    init(_ queue: OperationQueue) {
        workQueue = queue
        concurrentQueue = DispatchQueue(label: "com.zeroc.demo.ice.async.server.workqueue",
                                        qos: .default,
                                        attributes: .concurrent)
        workQueue.underlyingQueue = concurrentQueue
    }

    private func lock(cb: () -> Void) {
        doneSemaphore.wait()
        cb()
        doneSemaphore.signal()
    }

    private func addToQueue(delay: Int32, seal: Resolver<Void>) {
        lock {
            guard done == false else {
                seal.reject(RequestCanceledException())
                return
            }
            self.workQueue.addOperation {
                Thread.sleep(forTimeInterval: TimeInterval(delay) / 1000)
                self.lock {
                    guard self.done == false else {
                        seal.reject(RequestCanceledException())
                        return
                    }
                    print("Belated Hello World!")
                    seal.fulfill(())
                }
            }
        }
    }

    func sayHelloAsync(delay: Int32, current _: Current) -> Promise<Void> {
        return Promise { seal in
            if delay == 0 {
                print("Hello World!")
                seal.fulfill(())
            } else {
                addToQueue(delay: delay, seal: seal)
            }
        }
    }

    func shutdown(current: Current) throws {
        print("Shutting down...")
        lock { done = true }
        current.adapter?.getCommunicator().shutdown()
    }
}

func run() -> Int32 {
    do {
        var args = CommandLine.arguments
        let communicator = try Ice.initialize(args: &args, configFile: "config.server")

        defer {
            communicator.destroy()
        }
        guard args.count == 1 else {
            print("too many arguments\n")
            return 1
        }

        let workQueue = OperationQueue()
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
