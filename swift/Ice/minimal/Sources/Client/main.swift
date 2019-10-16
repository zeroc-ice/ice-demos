import Foundation
import Ice

do {
    let communicator = try Ice.initialize(CommandLine.arguments)
    defer {
        communicator.destroy()
    }

    let hello = try uncheckedCast(prx: communicator.stringToProxy("hello:default -h localhost -p 10000")!,
                                  type: HelloPrx.self)
    try hello.sayHello()
} catch {
    print("Error: \(error)\n")
    exit(1)
}
