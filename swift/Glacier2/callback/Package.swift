// swift-tools-version: 6.1

import PackageDescription

let package = Package(
    name: "glacier2-callback",
    platforms: [
        .macOS(.v15)
    ],
    dependencies: [
        .package(url: "https://download.zeroc.com/nexus/repository/nightly/ice-swift.git", branch: "main"),
        .package(name: "common", path: "../../common"),
    ],
    targets: [
        .executableTarget(
            name: "Client",
            dependencies: [
                .product(name: "Ice", package: "ice-swift"), .product(name: "Glacier2", package: "ice-swift"),
                .product(name: "DateExtension", package: "common"),
            ],
            exclude: ["slice-plugin.json"],
            plugins: [.plugin(name: "CompileSlice", package: "ice-swift")]
        ),
        .executableTarget(
            name: "Server",
            dependencies: [
                .product(name: "Ice", package: "ice-swift"), .product(name: "DateExtension", package: "common"),
            ],
            exclude: ["slice-plugin.json"],
            plugins: [.plugin(name: "CompileSlice", package: "ice-swift")]
        ),
    ]
)
