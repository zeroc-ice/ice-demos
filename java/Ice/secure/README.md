# Ice Secure

This demo illustrates how to programmatically configure client and server applications to use SSL secure connections.

## Building the Project

This project uses Gradle and the [Application plugin]. To build the client and server applications, run:

```shell
./gradlew build
```

## Running the Server

First, start the server application:

```shell
./gradlew :server:run --quiet --args="--Ice.Trace.Network"
```

## Running the Client

Then, in a separate terminal, start the client application:

```shell
./gradlew :client:run --quiet --args="--Ice.Trace.Network"
```

[Application plugin]: https://docs.gradle.org/current/userguide/application_plugin.html

> [!NOTE]
> The `--args="--Ice.Trace.Network"` command-line option turns on Network tracing. For this demo, it shows you that the
> requests are sent using the `ssl` secure transport.
