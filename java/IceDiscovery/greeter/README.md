# IceDiscovery Greeter

This demo illustrates how to configure the IceDiscovery plug-in. The IceDiscovery plug-in allows a client application
to discover Ice objects without hardcoding any addressing information.

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
./gradlew :client:run --quiet
```

[Application plugin]: https://docs.gradle.org/current/userguide/application_plugin.html
