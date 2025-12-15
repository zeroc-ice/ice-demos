# Glacier2 Greeter

This demo shows how to write a client that calls an Ice object hosted in a server "behind" a Glacier2 router:

```mermaid
flowchart LR
    c[Client] --tcp--> g[Glacier2 router:4063] --tcp--> s[Server:4061<br>hosts Greeter]
```

In a typical Glacier2 deployment, the client can establish a connection to the Glacier2 router but cannot establish
a connection to the server, because the server is on a separate network.

## Ice prerequisites

- Install Glacier2. See [Glacier2 installation].

## Building the demo

To build the demo, run:

```shell
swift build
```

## Running the demo

First, start the Server program in its own terminal:

```shell
swift run Server
```

Then, start the Glacier2 router in its own terminal:

```shell
glacier2router --Ice.Config=glacier2.conf
```

> [!TIP]
> You can also start the Glacier2 router before the server. The order does not matter: the server is identical to the
> server provided in the [Ice Greeter][1] demo and does not depend on Glacier2.

Finally, run the client application:

```shell
swift run Client
```

[Glacier2 installation]: https://zeroc.com/ice/downloads/3.8/glacier2

[1]: ../../Ice/Greeter
