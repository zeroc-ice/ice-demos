# Greeter

The Greeter demo illustrates how to send a request and wait for the response.

This demo provides two implementations for the server: a synchronous dispatch implementation (in server), and an
asynchronous dispatch implementation (in serveramd). The same client works with both.

You can build the client and server applications with:

``` shell
./gradlew build
```

First start the Server program:

```shell
./gradlew :server:run
```

or

```shell
./gradlew :serveramd:run
```

In a separate terminal, start the Client program:

```shell
./gradlew :client:run
```
