// swift-tools-version: 5.9

import Foundation
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
