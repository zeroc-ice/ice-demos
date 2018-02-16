# Objective-C Demos

## Overview

This directory contains Objective-C sample programs for various Ice components.
These examples are provided to get you started on using a particular Ice feature
or coding technique.

Most of the subdirectories here correspond directly to Ice components, such as
[Ice](./Ice) and [IceDiscovery](./IceDiscovery). We've also included the
following additional subdirectories:

- [Database](./Database)<p>
Ice clients for our Java-based [Database library](../java/Database/library) demo.

- [Manual](./Manual)<p>
Complete examples for some of the code snippets in the [Ice manual][1].

## Build Instructions

### Prerequisites

The iOS and Cocoa demos require the [Ice Builder for Xcode][2].

The command-line demos require the Xcode Command Line Tools to be installed
(use `xcode-select --install` to install them).

### Building the Demos

To build the command-line examples, first review the settings found in
`../make/Make.rules` and adjust any you want changed. If you've installed Ice
in a non-standard location, you'll also need to set the `ICE_HOME` environment
variable with the path name of the installation directory:

```
export ICE_HOME=~/testing/Ice
```

Run `make` to start the build:

```
make
```

To build the iOS or Cocoa examples, open `Objective-C demos.xcworkspace` in
Xcode. You will need to select a development team in the Xcode project `General`
settings of each demo to test the demos on an iOS device or macOS.

## Running the Demos

Refer to the README.md file in each demo directory for usage instructions.

[1]: https://doc.zeroc.com/display/Ice37/Ice+Manual
[2]: https://github.com/zeroc-ice/ice-builder-xcode
