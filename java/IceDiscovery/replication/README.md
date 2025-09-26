# IceDiscovery Replication

This demo illustrates how to configure the IceDiscovery plug-in. The IceDiscovery plug-in allows a client application
to discover Ice objects without hardcoding any addressing information.

## Building the demo

The demo has two Gradle projects, **client** and **server**, both using the [application plugin].

To build the demo, run:

```shell
./gradlew build
```

## Running the demo

First, start two or more server applications, each in its own terminal:

```shell
./gradlew :server:run --quiet --args="--Ice.Trace.Locator"
```

Then, in a separate terminal, start the client application:

```shell
./gradlew :client:run --quiet --args="--Ice.Trace.Locator"
```

>[!NOTE]
> The `--Ice.Trace.Locator` command-line option is optional: it turns on tracing (logging) for locator resolution and
> helps you understand the locator logic implemented by the IceDiscovery plug-in.

[Application plugin]: https://docs.gradle.org/current/userguide/application_plugin.html
