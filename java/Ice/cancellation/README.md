# Ice Cancellation

The Cancellation demo shows how to cancel an invocation by interrupting the thread waiting for a response, or by
cancelling the future returned by an async invocation. It also shows a related feature: invocation timeouts.

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
./client/build/install/client/bin/client --Ice.Trace.Network
```

> [!NOTE]
> The `--Ice.Trace.Network` command-line option turns on Network tracing. For this demo, it shows you that
> `InvocationTimeoutException` and `CancellationException` do not close the connection.

[Application plugin]: https://docs.gradle.org/current/userguide/application_plugin.html
