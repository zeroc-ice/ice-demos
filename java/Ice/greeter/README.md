# Ice Greeter

The Greeter demo illustrates how to send a request and wait for the response.

This demo provides two implementations for the server:

- `server`: A synchronous dispatch implementation
- `serveramd`: An asynchronous dispatch implementation using Ice's Asynchronous Method Dispatch (AMD)

The same client works with both.

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

or

```shell
./gradlew :serveramd:run --quiet
```

## Running the Client

Then, in a separate terminal, start the client application:

```shell
./gradlew :client:run --quiet
```

[Application plugin]: https://docs.gradle.org/current/userguide/application_plugin.html
