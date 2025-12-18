// swift-tools-version: 6.1

import PackageDescription

let package = Package(
    name: "context",
    platforms: [
        .macOS(.v15)
    ],
    dependencies: [.package(url: "https://github.com/zeroc-ice/ice.git", branch: "3.8")],
    targets: [
        .executableTarget(
            name: "Client",
            dependencies: [.product(name: "Ice", package: "ice")],
            plugins: [.plugin(name: "CompileSlice", package: "ice")]
        ),
        .executableTarget(
            name: "Server",
            dependencies: [.product(name: "Ice", package: "ice")],
            plugins: [.plugin(name: "CompileSlice", package: "ice")]
        ),
    ]
)
