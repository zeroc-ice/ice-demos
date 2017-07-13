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

To build the demos, open a command window and run:

```
gradlew build
```

The build will use a local Ice installation if it resides in a standard
location.

If you've installed Ice in a non-standard location, you'll first need to set the
`ICE_HOME` environment variable with the path name of the installation
directory.

To use a custom Maven repository for the Ice JAR files, pass the
`-PdevRepo` option to Gradle:

```
gradlew -PdevRepo=<URL> build
```

## Running the Demos

Before running a demo, make sure you've configured your environment to use Ice
as described in the [release notes][3].

Refer to the README file in each demo directory for usage instructions.

[1]: https://doc.zeroc.com/display/Ice37/Ice+Manual
[2]: https://zeroc.com/chat/index.html
[3]: https://doc.zeroc.com/display/Rel/Ice+3.7.0+Release+Notes
