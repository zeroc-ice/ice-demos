# Ice Cancellation

The Cancellation demo shows how to cancel an invocation by interrupting the thread waiting for a response, or by
cancelling the future returned by an async invocation. It also shows a related feature: invocation timeouts.

## Building the Project

This project uses Gradle and the [Application plugin]. To build the client and server applications, run:

```shell
./gradlew build
```

This will compile each project and install the distributions in the `build/install` subdirectory of each project.

## Running the Server

First, start the server application:

```shell
./gradlew :server:run --quiet
```

## Running the Client

Then, in a separate terminal, start the client application:

```shell
./gradlew :client:run --quiet --args="--Ice.Trace.Network"
```

> [!NOTE]
> The `--Ice.Trace.Network` command-line option turns on Network tracing. For this demo, it shows you that
> `InvocationTimeoutException`, `OperationInterruptedException`, and `CancellationException` do not close the connection.

[Application plugin]: https://docs.gradle.org/current/userguide/application_plugin.html
