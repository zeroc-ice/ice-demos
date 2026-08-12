# Ice Android Greeter

This demo illustrates how to create an Ice client application using Kotlin and Ice for Java.

The demo provides a simple Android client that can be used with any of the Greeter servers included in this repository.

## Prerequisites

- Android Studio
- An Android emulator or physical Android device running Android 14 (API level 34) or later

## Getting Started

First, start a Greeter server implemented in a language with server-side support (C++, C#, Java, Python, or Swift), for
example the [Java Greeter server](../greeter): follow the instructions in its README to start this server.

Then, open Android Studio and import the `java/Ice/android-greeter` project.
You can run the application on either an Android emulator or a physical Android device.

> [!NOTE]
> The Server IP Address field defaults to `10.0.2.2`, the address the Android emulator uses to reach the computer it
> runs on. This address only works in the emulator: on a physical Android device, enter the LAN address of the
> computer running the Greeter server (for example, `192.168.1.10`); the device and this computer must be on the same
> network.
