# Swift Demos

- [Swift Demos](#swift-demos)
  - [Overview](#overview)
  - [Building and Running the Demos](#building-and-running-the-demos)
    - [Build Requirements](#build-requirements)
    - [Building the Demos](#building-the-demos)
    - [Running the Demos](#running-the-demos)
  - [Ice Services](#ice-services)
  - [iOS Demos](#ios-demos)

## Overview

This directory contains Swift sample programs for various Ice components. These examples are provided to get you started
on using a particular Ice feature or coding technique.

Most of the subdirectories here correspond directly to Ice components, such as [Glacier2],
[IceStorm], and so on. We've also included the following additional sub-directories:

- [Manual] contains complete examples for some of the code snippets in the [Ice manual].

Refer to the [C++11 demos] for more examples that use the Ice services (Glacier2, IceGrid, IceStorm).

## Building and Running the Demos

### Build Requirements

- Xcode with Swift 5.5+
- Slice to Swift compiler (`slice2swift`)
- [Ice for C++] installed via Homebrew or from source

Install Ice using Homebrew:

```shell
brew install ice
```

### Building the Demos

To build a demo, navigate to the demo directory and use Make to generate the Slice files, then build with Swift
Package Manager:

```shell
cd Ice/hello
make slice
swift build
```

### Running the Demos

Refer to the README.md file in each demo directory for specific usage instructions.

## Ice Services

Some demos require Ice services such as IceGrid and IceStorm that must be running before you can run the demo. The
simplest approach is to install the Ice Homebrew packages:

```shell
brew install ice
```

Please refer to [Using the macOS Binary Distributions] for additional information.

## iOS Demos

The iOS demos (`Ice/helloUI` and `IceDiscovery/helloUI`) can be built using Xcode.

[C++11 demos]: ../cpp11
[Glacier2]: ./Glacier2
[IceStorm]: ./IceStorm
[Manual]: ./Manual
[Ice manual]: https://doc.zeroc.com/ice/3.7/introduction
[Ice for C++]: https://zeroc.com/downloads/ice
[Using the macOS Binary Distributions]: https://doc.zeroc.com/ice/3.7/release-notes/using-the-macos-binary-distribution
