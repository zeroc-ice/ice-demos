//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Foundation
import Ice

// Recursively print the contents of directory "dir" in tree fashion.
// For files, show the contents of each file. The "depth"
// parameter is the current nesting level (for indentation).
func listRecursive(dir: DirectoryPrx, depth: Int = 0) throws {
    let indent = String(repeating: "\t", count: depth + 1)
    let contents = try dir.list()
    for node in contents {
        if let subdir = try checkedCast(prx: node!, type: DirectoryPrx.self) {
            try print("\(indent)\(node!.name()) (directory):")
            try listRecursive(dir: subdir, depth: depth + 1)
        } else {
            let file = uncheckedCast(prx: node!, type: FilePrx.self)
            try print("\(indent)\(node!.name()) (file):")
            let lines = try file.read()
            for line in lines {
                print("\(indent)\t\(line)")
            }
        }
    }
}

func run() -> Int32 {
    do {
        let communicator = try Ice.initialize(CommandLine.arguments)
        defer {
            communicator.destroy()
        }

        //
        // Create a proxy for the root directory and down-cast the proxy to a Directory proxy
        //
        guard let obj = try communicator.stringToProxy("RootDir:default -h localhost -p 10000"),
            let rootDir = try checkedCast(prx: obj, type: DirectoryPrx.self) else {
            print("Invalid proxy")
            return 1
        }

        //
        // Recursively list the contents of the root directory
        //
        print("Contents of root directory:")
        try listRecursive(dir: rootDir)

        return 0
    } catch {
        print("Error: \(error)")
        return 1
    }
}

exit(run())
