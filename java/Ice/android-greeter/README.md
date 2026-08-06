# Ice Android Greeter

This demo illustrates how to create an Ice client application using Kotlin and Ice for Java.

The demo provides a simple Android client that can be used with any of the Greeter servers included in this repository.

## Getting Started

First, start a Greeter server implemented in a language with server-side support (C++, C#, Java, Python, or Swift), for
example the [Java Greeter server](../greeter): follow the instructions in its README to start this server.

Then, open Android Studio and import the `java/Ice/android-greeter` project.
You can run the application on either an Android emulator or a physical Android device.

> [!NOTE]
> The default IP address `10.0.2.2` is used to communicate with the host machine when running in the Android emulator.
> If your Greeter server is running on a different host, be sure to update the IP address in the app accordingly.
