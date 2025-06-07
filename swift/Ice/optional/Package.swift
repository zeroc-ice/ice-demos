// swift-tools-version: 5.9

import Foundation
import PackageDescription

let package = Package(
    name: "optional",
    platforms: [
        .macOS(.v15)
    ],
    dependencies: [.package(url: "https://download.zeroc.com/nexus/repository/nightly/ice-swift.git", branch: "main")],
    targets: [
        .executableTarget(
            name: "Client1",
            dependencies: [.product(name: "Ice", package: "ice-swift")],
            exclude: ["slice-plugin.json"],
            plugins: [.plugin(name: "CompileSlice", package: "ice-swift")]
        ),
        .executableTarget(
            name: "Client2",
            dependencies: [.product(name: "Ice", package: "ice-swift")],
            exclude: ["slice-plugin.json"],
            plugins: [.plugin(name: "CompileSlice", package: "ice-swift")]
        ),
        .executableTarget(
            name: "Server1",
            dependencies: [.product(name: "Ice", package: "ice-swift")],
            exclude: ["slice-plugin.json"],
            plugins: [.plugin(name: "CompileSlice", package: "ice-swift")]
        ),
        .executableTarget(
            name: "Server2",
            dependencies: [.product(name: "Ice", package: "ice-swift")],
            exclude: ["slice-plugin.json"],
            plugins: [.plugin(name: "CompileSlice", package: "ice-swift")]
        ),
    ]
)
