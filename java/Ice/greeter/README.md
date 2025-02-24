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

## Running the Servers

First, start one of the server implementations:

```shell
./server/build/install/server/bin/server
```

or

```shell
./serveramd/build/install/serveramd/bin/serveramd
```

## Running the Client

In a separate terminal, start the client program:

```shell
./client/build/install/client/bin/client
```

[Application plugin]: https://docs.gradle.org/current/userguide/application_plugin.html
