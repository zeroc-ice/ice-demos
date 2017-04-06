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
  and then start a private chat.

- [talk](./talk)

  An app that allows two devices to talk via Bluetooth.

Please review the build instructions below prior to opening the project.

# Build Instructions

This Android Studio project uses Gradle for its build system.

## Prerequisites

### Android Development Tools

Building any Ice application for Android requires Android Studio and the Android
SDK build tools. We tested with the following components:

- Android Studio 2.4 (preview)
- Android SDK Build-tools 26.0.0 rc1

Ice for Java requires at minimum API level 24:

- Android Nougat 7.0 (API24)

If you want to target a different version of the Android API level, edit
`gradle.properties`.

### Ice Development Kit

An Ice Development Kit is also required. Specifically, this project needs the
Ice JAR files along with the Slice to Java translator. The Gradle build system
will automatically search for your Ice Development Kit in the default
installation directories used by our [binary distributions][1]. If you've
installed Ice into a non-standard directory, or you're using a repository build,
edit `gradle.properties` and set the following variable:

    iceHome=<path to your Ice installation>

This path name must only use forward slashes, even on Windows.

## Building the Project

Follow these steps to import the project into Android Studio:

1. Start Android Studio
2. Select Open Project
3. Navigate to the "Android" subdirectory
4. If presented with an "Import Project from Gradle" dialog, select
   "Use default gradle wrapper" and press OK 
5. Select one of the subprojects from the drop down menu

[1]: https://zeroc.com/download.html
