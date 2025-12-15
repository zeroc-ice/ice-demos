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
    Server(Server<br/>hosts Greeter)
    Registry <--> Node --> | activate | Server
    Locator -.-> |return greeter endpoints | Client
    Client ==> |greet request| Server
```

## Prerequisites

- Install uv. See [Installing uv].
- Install IceGrid. See [IceGrid installation].

## Running the server with IceGrid

### 1. Compile the Slice definitions

Navigate to the `server` directory and use the Slice-to-Python compiler to generate Python code from the `Greeter.ice`
file:

```shell
uv run slice2py ../slice/Greeter.ice
```

### 2. Start the IceGrid registry

In a separate terminal, run:

```shell
icegridregistry --Ice.Config=registry.conf
```

### 3. Start the IceGrid node

In another terminal, first activate the virtual environment, then start the node:

#### macOS and Linux

```shell
source server/.venv/bin/activate
icegridnode --Ice.Config=node.conf
```

#### Windows (PowerShell)

```powershell
server\.venv\Scripts\activate
icegridnode --Ice.Config=node.conf
```

> [!NOTE]
> We need to start `icegridnode` from an active Python virtual environment in order for it to find the server program
> dependencies installed in that environment.

### 4. Deploy the Greeter application

Use the IceGrid admin tool to deploy the configuration:

```shell
icegridadmin --Ice.Config=admin.conf -e "application add greeter-hall.xml"
```

`greeter-hall.xml` configures a single Greeter server.
As an alternative, you can deploy 3 replicated Greeter servers with:

```shell
icegridadmin --Ice.Config=admin.conf -e "application add greeter-hall-with-replication.xml"
```

## Running the client

In a separate terminal, navigate to the `client` directory.

### 1. Compile the Slice definitions

Use the Slice-to-Python compiler to generate Python code from the `Greeter.ice` file:

```shell
uv run slice2py ../slice/Greeter.ice
```

### 2. Run the client

```shell
uv run main.py
```

[Installing uv]: https://docs.astral.sh/uv/getting-started/installation/
[IceGrid installation]: https://zeroc.com/ice/downloads/3.8/icegrid
