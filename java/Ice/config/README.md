# Ice Config

This demo shows how to configure client and server applications using Ice configuration files.

The same client works with both.

## Building the Project

This project uses Gradle and the [Application plugin]. To build the client and server applications, run:

```shell
./gradlew build
```

This will compile each project and install the distributions in the `build/install` subdirectory of each project.

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
