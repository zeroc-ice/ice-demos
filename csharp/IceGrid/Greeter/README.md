# IceGrid Greeter

The IceGrid Greeter demo illustrates how to create a very simple IceGrid deployment that manages a Greeter server.

```mermaid
flowchart LR
    Client(Client) --> | locate greeter | Locator
    subgraph Registry[IceGrid Registry]
        direction LR
        Locator[locator:4061]
    end
    Node[IceGrid Node]
    Server(Server<br/>hosts Chatbot)
    Registry <--> Node --> | activate | Server
    Locator -.-> |return greeter endpoints | Client
    Client ==> |greet request| Server
```

## Ice prerequisites

- Install IceGrid. See [Ice service installation].

## Building the demo

To build the demo, run:

```shell
dotnet build
```

## Running the demo

First, start the IceGrid registry in its own terminal:

```shell
icegridregistry --Ice.Config=config.registry
```

Then, start the IceGrid node in its own terminal:

```shell
icegridnode --Ice.Config=config.node
```

Next, deploy the "GreeterHall" application in this IceGrid deployment:

```shell
icegridadmin --Ice.Config=config.admin -e "application add greeter-hall.xml"
```

`greeter-hall.xml` configures a single Greeter server. As an alternative, you can deploy 3 replicated Greeter servers
with:

```shell
icegridadmin --Ice.Config=config.admin -e "application add greeter-hall-with-replication.xml"
```

> [!TIP]
> Use `update` instead of `add` to update an existing application.

Finally, run the client application:

```shell
cd Client
dotnet run
```

[Ice service installation]: https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#ice-services
