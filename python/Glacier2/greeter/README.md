# Glacier2 Greeter

This demo shows how to write a client that calls an Ice object hosted in a server "behind" a Glacier2 router:

```mermaid
flowchart LR
    c[Client] --tcp--> g[Glacier2 router:4063] --tcp--> s[Server:4061<br>hosts Greeter]
```

In a typical Glacier2 deployment, the client can establish a connection to the Glacier2 router but cannot connect
directly to the server, because the server is on a separate network.

## Prerequisites

- Install uv. See [Installing uv].
- Install Glacier2. See [Glacier2 installation].

## Running the server

Navigate to the `server` directory.

### 1. Compile the Slice definitions

Use the Slice-to-Python compiler to generate Python code from the `Greeter.ice` file:

```shell
uv run slice2py ../slice/Greeter.ice
```

### 2. Run the server

```shell
uv run main.py
```

### 3. Start the Glacier2 router

In a separate terminal, run:

```shell
glacier2router --Ice.Config=glacier2.conf
```

> [!TIP]
> You can start the Glacier2 router before or after the server.
> The server is identical to the one in the [Ice Greeter] demo and does not depend on Glacier2.

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
[Ice Greeter]: ../../Ice/greeter
[Glacier2 installation]: https://zeroc.com/ice/downloads/3.8/glacier2
