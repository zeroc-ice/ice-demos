# Ice Cancellation

The Cancellation demo shows how to cancel an invocation by interrupting the thread waiting for a response. It also shows
a related feature: invocation timeouts.

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
./gradlew :client:run --quiet
```

[Application plugin]: https://docs.gradle.org/current/userguide/application_plugin.html
