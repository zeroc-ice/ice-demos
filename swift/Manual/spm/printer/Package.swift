// swift-tools-version:5.0
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "PrinterDemo",
    platforms: [
        .macOS(.v10_13),
        .iOS(.v12)
    ],
    products: [
        .executable(name: "Server", targets: ["Server"]),
        .executable(name: "Client", targets: ["Client"])
    ],
    dependencies: [
        .package(url: "https://github.com/mxcl/PromiseKit.git", from: "6.8.10"),
        .package(url: "https://github.com/zeroc-ice/ice-spm.git", "3.7.3" ..< "3.8.0")
    ],
    targets: [
        .target(name: "Client", dependencies: ["Ice", "PromiseKit"]),
        .target(name: "Server", dependencies: ["Ice", "PromiseKit"])
    ]
)
