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

Follow these steps to build and run the demo:

1\. Build the client and server applications:

```shell
dotnet build
```

2\. Start the IceGrid registry in its own terminal:

```shell
icegridregistry --Ice.Config=config.registry
```

3\. Start the IceGrid node in its own terminal:

```shell
icegridnode --Ice.Config=config.node
```

4\. Deploy the "GreeterHall" application in this IceGrid deployment:

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

5\. Run the client application:

```shell
cd Client
dotnet run
```
