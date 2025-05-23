// swift-tools-version: 5.9

import Foundation
import PackageDescription

let package = Package(
    name: "greeter",
    platforms: [
        .macOS(.v14)
    ],
    dependencies: [.package(url: "https://download.zeroc.com/nexus/repository/nightly/ice-swift.git", branch: "main")],
    targets: [
        .executableTarget(
            name: "Client",
            dependencies: [.product(name: "Ice", package: "ice-swift")],
            exclude: ["slice-plugin.json"],
            plugins: [.plugin(name: "CompileSlice", package: "ice-swift")]
        ),
        .executableTarget(
            name: "Server",
            dependencies: [.product(name: "Ice", package: "ice-swift")],
            exclude: ["slice-plugin.json"],
            plugins: [.plugin(name: "CompileSlice", package: "ice-swift")]
        ),
        .executableTarget(
            name: "ServerAMD",
            dependencies: [.product(name: "Ice", package: "ice-swift")],
            exclude: ["slice-plugin.json"],
            plugins: [.plugin(name: "CompileSlice", package: "ice-swift")]
        ),
    ]
)
