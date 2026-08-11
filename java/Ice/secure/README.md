# Ice Secure

This demo illustrates how to programmatically configure client and server applications to use SSL secure connections.

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
./server/build/install/server/bin/server --Ice.Trace.Network
```

Then, in a separate terminal, start the client application:

```shell
./client/build/install/client/bin/client --Ice.Trace.Network
```

[Application plugin]: https://docs.gradle.org/current/userguide/application_plugin.html

> [!NOTE]
> The `--Ice.Trace.Network` command-line option turns on Network tracing. For this demo, it shows you that the
> requests are sent using the `ssl` secure transport.
