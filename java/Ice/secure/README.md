# Ice Secure

This demo illustrates how to programmatically configure client and server applications to use SSL secure connections.

## Building the demo

The demo has two Gradle projects, **client** and **server**, both using the [application plugin].

To build the demo, run:

```shell
./gradlew build
```

## Running the demo

First, start the server application:

```shell
./gradlew :server:run --quiet --args="--Ice.Trace.Network"
```

Then, in a separate terminal, start the client application:

```shell
./gradlew :client:run --quiet --args="--Ice.Trace.Network"
```

[Application plugin]: https://docs.gradle.org/current/userguide/application_plugin.html

> [!NOTE]
> The `--args="--Ice.Trace.Network"` command-line option turns on Network tracing. For this demo, it shows you that the
> requests are sent using the `ssl` secure transport.
