// swift-tools-version: 5.9

import Foundation
import PackageDescription

guard let iceHome = ProcessInfo.processInfo.environment["ICE_HOME"] else {
    fatalError("ICE_HOME environment variable not set")
}

let package = Package(
    name: "clock",
    platforms: [
        .macOS(.v14)
    ],
    dependencies: [.package(name: "ice", path: iceHome)],
    targets: [
        .executableTarget(
            name: "Publisher",
            dependencies: [.product(name: "Ice", package: "ice"), .product(name: "IceStorm", package: "ice")],
            exclude: ["slice-plugin.json"],
            plugins: [.plugin(name: "CompileSlice", package: "ice")]
        ),
        .executableTarget(
            name: "Subscriber",
            dependencies: [.product(name: "Ice", package: "ice"), .product(name: "IceStorm", package: "ice")],
            exclude: ["slice-plugin.json"],
            plugins: [.plugin(name: "CompileSlice", package: "ice")]
        ),
    ]
)
