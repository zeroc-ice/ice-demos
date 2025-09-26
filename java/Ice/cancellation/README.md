# Ice Cancellation

The Cancellation demo shows how to cancel an invocation by interrupting the thread waiting for a response, or by
cancelling the future returned by an async invocation. It also shows a related feature: invocation timeouts.

## Building the demo

The demo has two Gradle projects, **client** and **server**, both using the [application plugin].

To build the demo, run:

```shell
./gradlew build
```

## Running the demo

First, start the server application:

```shell
./gradlew :server:run --quiet
```

Then, in a separate terminal, start the client application:

```shell
./gradlew :client:run --quiet --args="--Ice.Trace.Network"
```

> [!NOTE]
> The `--Ice.Trace.Network` command-line option turns on Network tracing. For this demo, it shows you that
> `InvocationTimeoutException` and `CancellationException` do not close the connection.

[Application plugin]: https://docs.gradle.org/current/userguide/application_plugin.html
