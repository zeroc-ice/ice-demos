// swift-tools-version: 6.1

import PackageDescription

let package = Package(
    name: "common",
    products: [
        .library(name: "DateExtension", targets: ["DateExtension"])
    ],
    targets: [
        .target(name: "DateExtension")
    ]
)
