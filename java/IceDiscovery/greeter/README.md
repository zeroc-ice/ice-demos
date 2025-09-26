# IceDiscovery Greeter

This demo illustrates how to configure the IceDiscovery plug-in. The IceDiscovery plug-in allows a client application
to discover Ice objects without hardcoding any addressing information.

## Building the Project

This project uses Gradle and the [Application plugin]. To build the client and server applications, run:

```shell
./gradlew build
```

## Running the Server

First, start one of the server applications:

```shell
./gradlew :server:run --quiet
```

## Running the Client

Then, in a separate terminal, start the client application:

```shell
./gradlew :client:run --quiet
```

[Application plugin]: https://docs.gradle.org/current/userguide/application_plugin.html
