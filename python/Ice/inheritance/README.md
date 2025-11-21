# Ice Inheritance

The Inheritance demo shows how to write a simple filesystem application using interface inheritance.

## Prerequisites

- Install uv. See [Installing uv].

## Running the server

Navigate to the `server` directory.

### 1. Compile the Slice definitions

Use the Slice-to-Python compiler to generate Python code from the `Filesystem.ice` file:

```shell
uv run slice2py ../slice/Filesystem.ice
```

### 2. Run the server

```shell
uv run main.py
```

## Running the client

In a separate terminal, navigate to the `client` directory.

### 1. Compile the Slice definitions

Use the Slice-to-Python compiler to generate Python code from the `Filesystem.ice` file:

```shell
uv run slice2py ../slice/Filesystem.ice
```

### 2. Run the client

```shell
uv run main.py
```

[Installing uv]: https://docs.astral.sh/uv/getting-started/installation/
