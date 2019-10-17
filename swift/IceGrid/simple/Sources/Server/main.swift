//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Foundation
import Ice

class HelloI: Hello {
    private var name: String

    init(name: String) {
        self.name = name
    }

    func sayHello(current _: Current) throws {
        print(name + " says Hello World!")
    }

    func shutdown(current: Current) throws {
        print(name + " shutting down...")
        current.adapter!.getCommunicator().shutdown()
    }
}

// Automatically flush stdout
setbuf(__stdoutp, nil)

func run() -> Int32 {
    do {
        var args = CommandLine.arguments
        let communicator = try Ice.initialize(&args)
        defer {
            communicator.destroy()
        }
        guard args.count == 1 else {
            print("too many arguments")
            return 1
        }
        let adapter = try communicator.createObjectAdapter("Hello")
        let properties = communicator.getProperties()
        let id = try Ice.stringToIdentity(properties.getProperty("Identity"))
        let servant = HelloDisp(HelloI(name: properties.getProperty("Ice.ProgramName")))
        try adapter.add(servant: servant, id: id)
        try adapter.activate()
        communicator.waitForShutdown()
        return 0
    } catch {
        print("Error: \(error)\n")
        return 1
    }
}

exit(run())
