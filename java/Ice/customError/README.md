# Ice Custom Error

The Custom error demo shows how to define an exception in Slice, and how to throw and catch this exception.

A Slice-defined exception should be seen as a custom error carried by the response instead of the expected return
value--there is naturally no throwing across the network.

> [!NOTE]
> On Windows, run all the commands below in Git Bash or PowerShell; they don't work in the cmd.exe Command Prompt.

## Building the demo

The demo has two Gradle projects, **client** and **server**, both using the [application plugin].

To build the demo, run:

```shell
./gradlew installDist
```

This creates a self-contained distribution under build/install/ for each application, with launcher scripts in its
bin/ directory.

## Running the demo

First, start the server application:

```shell
./server/build/install/server/bin/server
```

Then, in a separate terminal, start the client application:

```shell
./client/build/install/client/bin/client
```

[Application plugin]: https://docs.gradle.org/current/userguide/application_plugin.html
