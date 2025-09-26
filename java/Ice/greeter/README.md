# Ice Greeter

The Greeter demo illustrates how to send a request and wait for the response.

This demo provides two implementations for the server:

- `server`: A synchronous dispatch implementation
- `serveramd`: An asynchronous dispatch implementation using Ice's Asynchronous Method Dispatch (AMD)

The same client works with both.

## Building the demo

The demo has three Gradle projects, **client**, **server**, and **serveramd**, both using the [application plugin].

To build the demo, run:

```shell
./gradlew build
```

## Running the demo

First, start one of the server applications:

```shell
./gradlew :server:run --quiet
```

or

```shell
./gradlew :serveramd:run --quiet
```

Then, in a separate terminal, start the client application:

```shell
./gradlew :client:run --quiet
```
[Application plugin]: https://docs.gradle.org/current/userguide/application_plugin.html
