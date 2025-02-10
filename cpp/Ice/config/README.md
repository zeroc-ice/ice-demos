# Config

This demo shows how to configure your client and server applications using Ice configuration files.

To build the demo run:

```shell
cmake -B build
cmake --build build --config Release
```

To run the demo, first start the server:

**Linux/macOS:**

```shell
./build/server
```

**Windows:**

```shell
build\Release\server
```

In a separate window, start the client:

**Linux/macOS:**

```shell
./build/client
```

**Windows:**

```shell
build\Release\client
```

You can pass `--Ice` command-line options to set additional properties or override the properties set in the
configuration file.

For example:

**Linux/macOS:**

```shell
./build/server --Ice.Default.Protocol=ws
```

```shell
./build/client --Ice.Default.Protocol=ws --Ice.Trace.Network=2
```

**Windows:**

```shell
build\Release\server --Ice.Default.Protocol=ws
```

```shell
build\Release\client --Ice.Default.Protocol=ws --Ice.Trace.Network=2
```
