# Ice Forwarder

The Forwarder demo shows how to write a servant that forwards all requests it receives to another remote Ice object,
as-is.

The core of this demo is the generic Forwarding server. This server listens on tcp port 10000 and forwards all
requests to tcp port 4061 on the same host.

The demo also includes a Greeter client and server; these are the usual Greeter client and server, except the client
is configured to use port 10000:

```mermaid
flowchart LR
    c[Greeter Client] --> f[Forwarding Server:10000] --> s[Greeter Server:4061]
```

The Forwarding server is generic can be inserted between any client and server. In particular, the Forwarding server
does not use any Slice generated code.

## Building the Project

This project uses Gradle and the [Application plugin]. To build the client and server applications, run:

```shell
./gradlew build
```

This will compile each project.

## Running the Server

First, start the Forwarding server and Greeter server in separate terminals:

```shell
./gradlew :server:run --quiet --Ice.Trace.Dispatch
```

```shell
./gradlew :forwardingserver:run --quiet --Ice.Trace.Dispatch
```

## Running the Client

Then, in a separate terminal, start the client application:

```shell
./gradlew :client:run --quiet --Ice.Trace.Dispatch
```

[Application plugin]: https://docs.gradle.org/current/userguide/application_plugin.html
