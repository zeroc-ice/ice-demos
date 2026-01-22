// swift-tools-version: 5.5
//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import PackageDescription

let package = Package(
    name: "clock",
    platforms: [.macOS(.v12)],
    products: [
        .executable(name: "Publisher", targets: ["Publisher"]),
        .executable(name: "Subscriber", targets: ["Subscriber"]),
    ],
    dependencies: [
        .package(url: "https://github.com/zeroc-ice/ice-spm.git", from: "3.7.10"),
        .package(url: "https://github.com/mxcl/PromiseKit.git", from: "6.8.10"),
    ],
    targets: [
        .executableTarget(
            name: "Publisher",
            dependencies: [
                .product(name: "Ice", package: "ice-spm"),
                .product(name: "IceStorm", package: "ice-spm"),
                "PromiseKit",
            ],
            path: "Sources/Publisher"
        ),
        .executableTarget(
            name: "Subscriber",
            dependencies: [
                .product(name: "Ice", package: "ice-spm"),
                .product(name: "IceStorm", package: "ice-spm"),
                "PromiseKit",
            ],
            path: "Sources/Subscriber"
        ),
    ]
)
