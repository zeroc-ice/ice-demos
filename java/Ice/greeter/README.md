# Greeter

The Greeter demo illustrates how to send a request and wait for the response.

This demo provides two implementations for the server:
- `server`: A synchronous dispatch implementation
- `serveramd`: An asynchronous dispatch implementation using Ice's Asynchronous Method Dispatch (AMD)

The same client works with both.

## Building the Project

This project uses Gradle and the [Application plugin]. To build the client and server applications, run:

```shell
./gradlew installDist
```

This will compile each project and install the distributions in the `build/install` subdirectory of each project.

## Running the Server

First, start one of the server implementations:

```shell
./gradlew :server:run --quiet
```

or

```shell
./gradlew :serveramd:run --quiet
```

## Running the Client

Then, in a separate terminal, start the client program:

```shell
./gradlew :client:run --quiet
```

[Application plugin]: https://docs.gradle.org/current/userguide/application_plugin.html
