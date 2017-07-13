# Java Demos

## Overview

This directory contains Java sample programs for various Ice components. These
examples are provided to get you started on using a particular Ice feature or
coding technique.

Most of the subdirectories here correspond directly to Ice components, such as
[IceGrid](./IceGrid), [Glacier2](./Glacier2), and so on. We've also included the
following additional subdirectories:

- [Manual](./Manual) contains complete examples for some of the code snippets
in the [Ice manual][1].

- [Chat](./Chat) contains a GUI client for the ZeroC [Chat Demo][2].

- [Android](./Android) contains examples of Android applications.

Refer to the [C++11 demos](../cpp11) for more examples that use the Ice services
(Glacier2, IceGrid, IceStorm).

## Build Instructions

The build system for the Java sample programs uses [Gradle](http://gradle.org)
and includes a wrapper script or batch file that automatically downloads Gradle
and all other required components.

### Building the Demos on Windows

We recommending installing the Ice MSI, which includes the Slice-to-Java
compiler.

To build the demos, open a command window and run:

```
gradlew build
```

The default build assumes the Slice-to-Java compiler is installed in a standard
location. The build also downloads the Ice JAR files from Maven Central.

If you've built the Slice-to-Java compiler and Ice for Java in a source tree,
you'll need to pass some additional options to Gradle:

```
gradlew -PiceHome=<path to source tree> -PcppConfiguration=<Release|Debug> \
    -PcppPlatform=<Win32|x64> build
```

### Building the Demos on Linux and macOS

We recommending installing the binary distribution for your platform, which
includes the Slice-to-Java compiler.

To build the demos, open a command window and run:

```
gradlew build
```

The default build assumes the Slice-to-Java compiler is installed in a standard
location. The build also downloads the Ice JAR files from Maven Central if your
distribution did not include them.

If you've built the Slice-to-Java compiler and Ice for Java in a source tree,
you'll need to pass an additional option to Gradle:

```
gradlew -PiceHome=<path to source tree> build
```

## Running the Demos

Refer to the README file in each demo directory for usage instructions.

[1]: https://doc.zeroc.com/display/Ice37/Ice+Manual
[2]: https://zeroc.com/chat/index.html
