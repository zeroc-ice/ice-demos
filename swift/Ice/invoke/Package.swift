// swift-tools-version: 5.5
//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import PackageDescription

let package = Package(
    name: "invoke",
    platforms: [.macOS(.v12)],
    products: [
        .executable(name: "Client", targets: ["Client"]),
        .executable(name: "Server", targets: ["Server"]),
    ],
    dependencies: [
        .package(url: "https://github.com/zeroc-ice/ice-spm.git", from: "3.7.10"),
        .package(url: "https://github.com/mxcl/PromiseKit.git", from: "6.8.10"),
    ],
    targets: [
        .executableTarget(
            name: "Client",
            dependencies: [
                .product(name: "Ice", package: "ice-spm"),
                "PromiseKit",
            ],
            path: "Sources/Client"
        ),
        .executableTarget(
            name: "Server",
            dependencies: [
                .product(name: "Ice", package: "ice-spm"),
                "PromiseKit",
            ],
            path: "Sources/Server"
        ),
    ]
)
