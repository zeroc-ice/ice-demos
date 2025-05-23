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
            name: "Sensor",
            dependencies: [
                .product(name: "Ice", package: "ice-swift"), .product(name: "IceStorm", package: "ice-swift"),
            ],
            exclude: ["slice-plugin.json"],
            plugins: [.plugin(name: "CompileSlice", package: "ice-swift")]
        ),
        .executableTarget(
            name: "Station",
            dependencies: [
                .product(name: "Ice", package: "ice-swift"), .product(name: "IceStorm", package: "ice-swift"),
            ],
            exclude: ["slice-plugin.json"],
            plugins: [.plugin(name: "CompileSlice", package: "ice-swift")]
        ),
    ]
)
