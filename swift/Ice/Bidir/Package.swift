// swift-tools-version: 6.1

import PackageDescription

let package = Package(
    name: "bidir",
    platforms: [
        .macOS(.v15)
    ],
    dependencies: [
        .package(url: "https://github.com/zeroc-ice/ice.git", from: "3.8.0"),
        .package(name: "common", path: "../../common"),
    ],
    targets: [
        .executableTarget(
            name: "Client",
            dependencies: [
                .product(name: "Ice", package: "ice"), .product(name: "DateExtension", package: "common"),
            ],
            plugins: [.plugin(name: "CompileSlice", package: "ice")]
        ),
        .executableTarget(
            name: "Server",
            dependencies: [
                .product(name: "Ice", package: "ice"), .product(name: "DateExtension", package: "common"),
            ],
            plugins: [.plugin(name: "CompileSlice", package: "ice")]
        ),
    ]
)
