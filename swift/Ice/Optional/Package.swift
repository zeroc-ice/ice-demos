// swift-tools-version: 6.1

import PackageDescription

let package = Package(
    name: "optional",
    platforms: [
        .macOS(.v15)
    ],
    dependencies: [.package(url: "https://github.com/zeroc-ice/ice-swift-nightly.git", branch: "main")],
    targets: [
        .executableTarget(
            name: "Client1",
            dependencies: [.product(name: "Ice", package: "ice-swift-nightly")],
            plugins: [.plugin(name: "CompileSlice", package: "ice-swift-nightly")]
        ),
        .executableTarget(
            name: "Client2",
            dependencies: [.product(name: "Ice", package: "ice-swift-nightly")],
            plugins: [.plugin(name: "CompileSlice", package: "ice-swift-nightly")]
        ),
        .executableTarget(
            name: "Server1",
            dependencies: [.product(name: "Ice", package: "ice-swift-nightly")],
            plugins: [.plugin(name: "CompileSlice", package: "ice-swift-nightly")]
        ),
        .executableTarget(
            name: "Server2",
            dependencies: [.product(name: "Ice", package: "ice-swift-nightly")],
            plugins: [.plugin(name: "CompileSlice", package: "ice-swift-nightly")]
        ),
    ]
)
