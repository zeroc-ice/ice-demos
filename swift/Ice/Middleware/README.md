# Ice Middleware

The Middleware demo shows how to write a simple middleware and how to add this middleware to an object adapter.

Each object adapter maintains a dispatch pipeline:

```mermaid
flowchart LR
    client[Client] -- request --> am -- response --> client
    subgraph pipeline [Dispatch pipeline]
        direction LR
        am[Authorization <br>middleware] --> om[Other <br>middleware] --> Servant --> om --> am
    end
```

> [!NOTE]
> Other middleware is just an example, and is not provided by this demo.

## Building the demo

To build the demo, run:

```shell
swift build
```

## Running the demo

First start the Server program:

```shell
swift run Server
```

In a separate terminal, start the Client program:

```shell
swift run Client
```
