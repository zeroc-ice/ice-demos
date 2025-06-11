// swift-tools-version: 6.1

import PackageDescription

let package = Package(
    name: "weather",
    platforms: [
        .macOS(.v15)
    ],
    dependencies: [.package(url: "https://github.com/zeroc-ice/ice-swift-nightly.git", branch: "main")],
    targets: [
        .executableTarget(
            name: "Sensor",
            dependencies: [
                .product(name: "Ice", package: "ice-swift-nightly"),
                .product(name: "IceStorm", package: "ice-swift-nightly"),
            ],
            plugins: [.plugin(name: "CompileSlice", package: "ice-swift-nightly")]
        ),
        .executableTarget(
            name: "Station",
            dependencies: [
                .product(name: "Ice", package: "ice-swift-nightly"),
                .product(name: "IceStorm", package: "ice-swift-nightly"),
            ],
            plugins: [.plugin(name: "CompileSlice", package: "ice-swift-nightly")]
        ),
    ]
)
