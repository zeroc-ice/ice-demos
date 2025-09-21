# Ice Bidir

The Bidir demo illustrates how to send requests "the other way around", from a server to a client, by reusing the
connection established by the client to the server.

This demo is very similar to the [Callback][1] demo: with the Callback demo, the server opens a connection to the
client, while with this demo, there is only one connection, from the client to the server:

```mermaid
flowchart LR
    c[Client<br>hosts AlarmClock] --bidir connection--> s[Server:4061<br>hosts WakeUpService]
```

This is particularly useful when the client application is behind a firewall that does not allow incoming connections.

## Building the demo

To build the demo, run:

```shell
dotnet build
```

## Running the demo

First, start the Server program:

```shell
cd Server
dotnet run
```

Then, in a separate terminal, start the Client program:

```shell
cd Client
dotnet run
```

[1]: ../Callback
