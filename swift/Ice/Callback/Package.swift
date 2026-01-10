// swift-tools-version: 6.1

import PackageDescription

let package = Package(
    name: "callback",
    platforms: [
        .macOS(.v15)
    ],
    dependencies: [
        .package(url: "https://github.com/zeroc-ice/ice-swift-nightly.git", branch: "3.9"),
        .package(name: "common", path: "../../common"),
    ],
    targets: [
        .executableTarget(
            name: "Client",
            dependencies: [
                .product(name: "Ice", package: "ice-swift-nightly"), .product(name: "DateExtension", package: "common"),
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
