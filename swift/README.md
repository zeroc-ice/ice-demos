# Swift Demos

## Overview

This directory contains Swift sample programs for various Ice components. These
examples are provided to get you started on using a particular Ice feature or
coding technique.

Most of the subdirectories here correspond directly to Ice components, such as
[Glacier2](./Glacier2), [IceStorm](./IceStorm), and so on. We've also included
the following additional sub-directories:

- [Manual](./Manual) contains complete examples for some of the code snippets
in the [Ice manual][1].

- [iOS](./iOS) contains iOS sample applications.

Refer to the [C++11 demos](../cpp11) for more examples that use the Ice services
(Glacier2, IceGrid, IceStorm).

## Building and running the demos with Xcode and Carthage

### Build Requirements

In order to build these Ice for Swift sample programs, you need:
 * Xcode
 * Slice to Swift and Slice to C++ compilers
 * [Carthage][2]

### Install required dependencies using Carthage:

```
carthage update
```

This command will build the `Ice` and `PromiseKit` frameworks required by the
sample programs and install them in the `Carthage/Build/` directory.

Note: building `Ice macOS` and `Ice iOS` from source during `carthage update`
takes some time, please be patient.

If you want to use the `slice2swift` and `slice2cpp` compilers from a source
distribution, set the `ICE_HOME` environment variable before running
`carthage update`.

### Building the demos

Open the project file `demos.xcodeproj` to build the sample programs.

The demos are configured to use the `Ice` and `PromiseKit` frameworks from the
Carthage builds and the `slice2swift` compiler from the binary distribution.

If you want to build the sample programs using `slice2swift` compiler from a
source distribution, set the `ICE_HOME` environment variable before starting
Xcode.

### Running the Demos

For most demos, you can simply run `server` and `client` in separate terminals.
Refer to the README.md file in each demo directory for the exact usage
instructions.

Some demos require Ice services such as IceGrid and IceStorm that are not
included in the Ice for Swift distribution. To run these demos, the simplest
is to first install the Ice Homebrew packages. Please refer to
[Using the macOS Binary Distributions][3] for additional information.

## Building and running the demos with Swift Package Manager (SPM)

*There is currently a single demo with SPM support, printer.*

### Building the demos

Run `swift build` from the demo directory.

```
cd Manual/spm/printer
swift build
```

### Running the demos

Swift Package Manager puts the executables in the build output directory. Refer
to the printer's [README.md](./Manual/spm/printer/README.md) for more
information.

[1]: https://doc.zeroc.com/ice/4.0/introduction
[2]: https://github.com/Carthage/Carthage
[3]: https://doc.zeroc.com/ice/4.0/release-notes/using-the-macos-binary-distribution
