# IceDiscovery Replication

This demo illustrates how to configure the IceDiscovery plug-in. The IceDiscovery plug-in allows a client application
to discover Ice objects without hardcoding any addressing information.

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

First, start two or more server applications, each in its own terminal:

```shell
./server/build/install/server/bin/server --Ice.Trace.Locator
```

Then, in a separate terminal, start the client application:

```shell
./client/build/install/client/bin/client --Ice.Trace.Locator
```

> [!NOTE]
> The `--Ice.Trace.Locator` command-line option is optional: it turns on tracing (logging) for locator resolution and
> helps you understand the locator logic implemented by the IceDiscovery plug-in.

[Application plugin]: https://docs.gradle.org/current/userguide/application_plugin.html
