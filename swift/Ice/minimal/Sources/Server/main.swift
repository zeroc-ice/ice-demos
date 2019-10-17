//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Foundation
import Ice

// Automatically flush stdout
setbuf(__stdoutp, nil)

struct HelloI: Hello {
    func sayHello(current _: Ice.Current) throws {
        print("Hello World!")
    }
}

do {
    let communicator = try Ice.initialize(CommandLine.arguments)
    defer {
        communicator.destroy()
    }

    let adapter = try communicator.createObjectAdapterWithEndpoints(name: "Hello",
                                                                    endpoints: "default -h localhost -p 10000")
    try adapter.add(servant: HelloDisp(HelloI()), id: Ice.stringToIdentity("hello"))
    try adapter.activate()
    communicator.waitForShutdown()
} catch {
    print("Error: \(error)\n")
    exit(1)
}
