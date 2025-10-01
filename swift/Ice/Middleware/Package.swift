// swift-tools-version: 6.1

import PackageDescription

let package = Package(
    name: "middleware",
    platforms: [
        .macOS(.v15)
    ],
    dependencies: [.package(url: "https://github.com/zeroc-ice/ice-swift-nightly.git", branch: "main")],
    targets: [
        .executableTarget(
            name: "Client",
            dependencies: [.product(name: "Ice", package: "ice-swift-nightly")],
            plugins: [.plugin(name: "CompileSlice", package: "ice-swift-nightly")]
        ),
        .executableTarget(
            name: "Server",
            dependencies: [.product(name: "Ice", package: "ice-swift-nightly")],
            plugins: [.plugin(name: "CompileSlice", package: "ice-swift-nightly")]
        ),
    ]
)
