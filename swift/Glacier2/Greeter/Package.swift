// swift-tools-version: 6.1

import PackageDescription

let package = Package(
    name: "glacier2-greeter",
    platforms: [
        .macOS(.v15)
    ],
    dependencies: [
        .package(url: "https://github.com/zeroc-ice/ice.git", .upToNextMinor(from: "3.8.0"))
    ],
    targets: [
        .executableTarget(
            name: "Client",
            dependencies: [
                .product(name: "Ice", package: "ice"),
                .product(name: "Glacier2", package: "ice"),
            ],
            plugins: [.plugin(name: "CompileSlice", package: "ice")]
        ),
        .executableTarget(
            name: "Server",
            dependencies: [
                .product(name: "Ice", package: "ice")
            ],
            plugins: [.plugin(name: "CompileSlice", package: "ice")]
        ),
    ]
)
