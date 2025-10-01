// swift-tools-version: 6.1

import PackageDescription

let package = Package(
    name: "glacier2-callback",
    platforms: [
        .macOS(.v15)
    ],
    dependencies: [
        .package(url: "https://github.com/zeroc-ice/ice-swift-nightly.git", branch: "main"),
        .package(name: "common", path: "../../common"),
    ],
    targets: [
        .executableTarget(
            name: "Client",
            dependencies: [
                .product(name: "Ice", package: "ice-swift-nightly"),
                .product(name: "Glacier2", package: "ice-swift-nightly"),
                .product(name: "DateExtension", package: "common"),
            ],
            plugins: [.plugin(name: "CompileSlice", package: "ice-swift-nightly")]
        ),
        .executableTarget(
            name: "Server",
            dependencies: [
                .product(name: "Ice", package: "ice-swift-nightly"), .product(name: "DateExtension", package: "common"),
            ],
            plugins: [.plugin(name: "CompileSlice", package: "ice-swift-nightly")]
        ),
    ]
)
