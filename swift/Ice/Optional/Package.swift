// swift-tools-version: 6.1

import PackageDescription

let package = Package(
    name: "optional",
    platforms: [
        .macOS(.v15)
    ],
    dependencies: [.package(url: "https://github.com/zeroc-ice/ice.git", .upToNextMinor(from: "3.8.0"))],
    targets: [
        .executableTarget(
            name: "Client1",
            dependencies: [.product(name: "Ice", package: "ice")],
            plugins: [.plugin(name: "CompileSlice", package: "ice")]
        ),
        .executableTarget(
            name: "Client2",
            dependencies: [.product(name: "Ice", package: "ice")],
            plugins: [.plugin(name: "CompileSlice", package: "ice")]
        ),
        .executableTarget(
            name: "Server1",
            dependencies: [.product(name: "Ice", package: "ice")],
            plugins: [.plugin(name: "CompileSlice", package: "ice")]
        ),
        .executableTarget(
            name: "Server2",
            dependencies: [.product(name: "Ice", package: "ice")],
            plugins: [.plugin(name: "CompileSlice", package: "ice")]
        ),
    ]
)
