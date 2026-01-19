# Java Compat Demos

## Overview

This directory contains Java sample programs for various Ice components. These examples are provided to get you started
on using a particular Ice feature or coding technique.

Most of the subdirectories here correspond directly to Ice components, such as [IceGrid](./IceGrid),
[Glacier2](./Glacier2), and so on. We've also included the following additional subdirectories:

- [android](./android) contains examples of Android applications.
- [Manual](./Manual) contains complete examples for some of the code snippets in the [Ice manual].

Refer to the [C++11 demos](../cpp11) for more examples that use the Ice services (Glacier2, IceGrid, IceStorm).

## Build Instructions

The build system for the Java sample programs uses [Gradle](http://gradle.org) and includes a wrapper script or batch
file that automatically downloads Gradle and all other required components.

### Building the Demos on Windows

We recommending installing the Ice MSI, which includes the Slice-to-Java compiler.

To build the demos, open a command window and run:

```shell
gradlew build
```

The default build assumes the Slice-to-Java compiler is installed in a standard location. The build also downloads the
Ice JAR files from Maven Central.

#### Building with nightly packages on Windows

You can build the demos using the latest Ice nightly packages from the ZeroC nightly repository.

First, install the slice2java nightly package following the instructions at
<https://github.com/zeroc-ice/ice/blob/3.7/NIGHTLY.md>.

Then, run the setup script from the repository root:

```shell
python scripts/setup-nightly.py --channel 3.7
```

Then activate the nightly environment and build:

```shell
.\scripts\nightly-env.ps1
gradlew build -PdevRepo="$env:MAVEN_REPO" -PiceArtifactVersion="3.7.11+"
```

To reset back to the release packages:

```shell
python scripts/setup-nightly.py --reset
```

### Building the Demos on Linux and macOS

We recommending installing the binary distribution for your platform, which includes the Slice-to-Java compiler.

To build the demos, open a command window and run:

```shell
./gradlew build
```

The default build assumes the Slice-to-Java compiler is installed in a standard location. The build also downloads the
Ice JAR files from Maven Central if your distribution did not include them.

#### Building with nightly packages on Linux and macOS

You can build the demos using the latest Ice nightly packages from the ZeroC nightly repository.

First, install the slice2java nightly package following the instructions at
<https://github.com/zeroc-ice/ice/blob/3.7/NIGHTLY.md>.

Then, run the setup script from the repository root:

```shell
python scripts/setup-nightly.py --channel 3.7
```

Then activate the nightly environment and build:

```shell
source scripts/nightly-env.sh
./gradlew build -PdevRepo="$MAVEN_REPO" -PiceArtifactVersion="3.7.11+"
```

To reset back to the release packages:

```shell
python scripts/setup-nightly.py --reset
```

### Building the Demos for Android

This Android Studio project uses Gradle for its build system.

#### Android Development Tools

Building any Ice application for Android requires Android Studio and the Android SDK build tools. We tested with the
following components:

- Android Studio Otter 3 Feature Drop
- Android SDK 36

Ice requires at minimum API level 21:

- Android 5 (API21)

If you want to target a different version of the Android API level, edit the project settings in Android Studio.

#### Slice-to-Java Compiler and Ice JAR Files

The build assumes the Slice-to-Java compiler is installed in a standard location. The build also downloads the Ice JAR
files from Maven Central.

#### Building the Android Project

Follow these steps to open the project in Android Studio:

1. Start Android Studio
2. Select "Open an existing Android Studio project"
3. Navigate to and select the "android" subdirectory
4. Click OK and wait for the project to open and build
5. Select one of the subprojects from the drop down menu

## Running the Demos

Refer to the README.md file in each demo directory for usage instructions.

[Ice manual]: https://doc.zeroc.com/ice/3.7/introduction
