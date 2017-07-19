# Overview

This directory contains the following Android examples:

- [chat](./chat)

  A client for the ZeroC chat application.

- [hello](./hello)

  An app that shows how to invoke ordinary (twoway)
  and oneway operations, as well as how to use datagrams, secure
  invocations, and batched invocations.

- [library](./library)

  A front-end for remotely querying and manipulating a database.

- [mtalk](./mtalk)

  An app that allows two devices to locate each other via UDP multicast
  and then start a private chat. This demo can only be run on a device;
  multicast is not supported in an emulator.

- [talk](./talk)

  An app that allows two devices to talk via Bluetooth. This demo can only
  be run on a device; Bluetooth is not supported in an emulator.

Please review the build instructions below prior to opening the project.

# Build Instructions

This Android Studio project uses Gradle for its build system.

## Prerequisites

### Android Development Tools

Building any Ice application for Android requires Android Studio and the Android
SDK build tools. We tested with the following components:

- Android Studio 3.0 preview
- Android SDK 24
- Android Build Tools 26.0.0

Android Studio 3.0 is required for its Java 8 support. Ice for Java cannot be
used with Android Studio 2.x.

Ice for Java requires at minimum API level 24:

- Android Nougat 7.0 (API24)

If you want to target a different version of the Android API level, edit the
project settings in Android Studio.

### Slice-to-Java Compiler

The easiest way to obtain the Slice-to-Java compiler is to install a [binary
Ice distribution][1] for your platform. The build assumes the distribution is
installed in a standard location. If you've installed it elsewhere, or you
want to use a source build instead, edit `gradle.properties` and set
`iceHome` to the location of your Ice installation.

### Ice JAR Files

The build automatically downloads the Ice JAR files from Maven Central. If
you want to use Ice JAR files from a local installation, such as a source
build, edit `gradle.properties` and set `iceHome` to the location of your Ice
installation.

## Building the Project

Follow these steps to open the project in Android Studio:

1. Start Android Studio
2. Select "Open an existing Android Studio project"
3. Navigate to and select the "android" subdirectory
4. Click OK and wait for the project to open and build
5. Select one of the subprojects from the drop down menu

[1]: https://zeroc.com/download.html
