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

The Forwarding server is generic and can be inserted between any client and server. In particular, the Forwarding server
does not use any Slice generated code.

## Building the Project

This project uses Gradle and the [Application plugin]. To build the client and server applications, run:

```shell
./gradlew build
```

## Running the Server

First, start the Forwarding server and Greeter server in separate terminals:

```shell
./gradlew :server:run --quiet --args="--Ice.Trace.Dispatch"
```

```shell
./gradlew :forwardingserver:run --quiet --args="--Ice.Trace.Dispatch"
```

## Running the Client

Then, in a separate terminal, start the client application:

```shell
./gradlew :client:run --quiet --args="--Ice.Trace.Network"
```

[Application plugin]: https://docs.gradle.org/current/userguide/application_plugin.html

> [!NOTE]
> The `--Ice.Trace` command-line options are optional: they turn-on tracing (logging) for request dispatches
> (`--Ice.Trace.Dispatch`) and connection establishment/closure (`--Ice.Trace.Network`) and help you follow the call
> flow.
