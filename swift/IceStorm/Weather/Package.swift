// swift-tools-version: 6.1

import PackageDescription

let package = Package(
    name: "weather",
    platforms: [
        .macOS(.v15)
    ],
    dependencies: [.package(url: "https://github.com/zeroc-ice/ice.git", .upToNextMinor(from: "3.8.0"))],
    targets: [
        .executableTarget(
            name: "Sensor",
            dependencies: [
                .product(name: "Ice", package: "ice"),
                .product(name: "IceStorm", package: "ice"),
            ],
            plugins: [.plugin(name: "CompileSlice", package: "ice")]
        ),
        .executableTarget(
            name: "Station",
            dependencies: [
                .product(name: "Ice", package: "ice"),
                .product(name: "IceStorm", package: "ice"),
            ],
            plugins: [.plugin(name: "CompileSlice", package: "ice")]
        ),
    ]
)
