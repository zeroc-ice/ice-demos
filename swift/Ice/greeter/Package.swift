// swift-tools-version: 5.9

import Foundation
import PackageDescription

guard let iceHome = ProcessInfo.processInfo.environment["ICE_HOME"] else {
    fatalError("ICE_HOME environment variable not set")
}

let package = Package(
    name: "greeter",
    platforms: [
        .macOS(.v14)
    ],
    dependencies: [.package(name: "ice", path: iceHome)],
    targets: [
        .executableTarget(
            name: "Client",
            dependencies: [.product(name: "Ice", package: "ice")],
            exclude: ["slice-plugin.json"],
            plugins: [.plugin(name: "CompileSlice", package: "ice")]
        ),
        .executableTarget(
            name: "Server",
            dependencies: [.product(name: "Ice", package: "ice")],
            exclude: ["slice-plugin.json"],
            plugins: [.plugin(name: "CompileSlice", package: "ice")]
        ),
    ]
)
