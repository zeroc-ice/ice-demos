// Copyright (c) ZeroC, Inc.

import Ice

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
var args = CommandLine.arguments
let communicator = try Ice.initialize(&args)

// Destroy the communicator when the program exits.
defer {
    communicator.destroy()
}

// Create a proxy for the root directory.
let rootDir = try makeProxy(
    communicator: communicator, proxyString: "RootDir:tcp -h localhost -p 4061",
    type: DirectoryPrx.self)

// Recursively list the contents of the root directory
print("Contents of root directory:")
try await listRecursive(dir: rootDir)

/// Recursively prints the contents of a directory in tree fashion. For files, show the contents of each file.
/// - Parameters:
///   - dir: The directory to list.
///   - depth: The current depth in the directory tree. Used for indentation.
func listRecursive(dir: DirectoryPrx, depth: Int = 0) async throws {
    let indent = String(repeating: "\t", count: depth + 1)
    let contents = try await dir.list()

    for node in contents {
        let node = node! // The node proxies returned by list() are never nil.

        let subdir = try await checkedCast(prx: node, type: DirectoryPrx.self)
        let kind = subdir != nil ? "directory" : "file"
        let nodeName = try await node.name()

        print("\(indent)\(nodeName) \(kind):")

        if let subdir = subdir {
            try await listRecursive(dir: subdir, depth: depth + 1)
        } else {
            let file = uncheckedCast(prx: node, type: FilePrx.self)
            let lines = try await file.read()
            for line in lines {
                print("\(indent)\t\(line)")
            }
        }
    }
}
