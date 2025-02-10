# IceGrid Registry Replication

This example demonstrates IceGrid's [registry replication][1] feature.

To build the demo run:

```shell
cmake -B build
cmake --build build --config Release
```

> [!NOTE]
> The IceGrid registries and nodes can be started in any order.

To run the demo, first start the IceGrid registry:

```shell
icegridregistry --Ice.Config=config.master
```

In a separate windows, start the first IceGrid registry replicas:

```shell
icegridregistry --Ice.Config=config.replica1
```

```shell
icegridregistry --Ice.Config=config.replica2
```

In separate windows, start the IceGrid nodes. Before running `icegridnode` add
the `server` build directory to the PATH environment variable:

**Linux/macOS:**

```shell
export PATH=$PWD/build:$PATH
```

**Windows:**

```shell
set PATH=%CD%\\build\\Release;%PATH%
```

Start the IceGrid nodes:

```shell
icegridnode --Ice.Config=config.node1
```

```shell
icegridnode --Ice.Config=config.node2
```

In a separate window, add the server application to the IceGrid registry:

```shell
icegridadmin --Ice.Config=config.client -e "application add application.xml"
```

Next, run the client:

**Linux/macOS:**

```shell
./build/client
```

**Windows:**

```shell
build\Release\client
```

The client invokes the number of specified iterations with a given
delay on a well-known proxy configured to use per-request load
balancing. Each invocation on the proxy queries the IceGrid registry
locator to retrieve the endpoints of the object (you can set
`Ice.Trace.Location=1` to see the locator invocations).

While the client is running and invoking on the server, you can try to
shutdown some of the registries with the `registry shutdown`
icegridadmin command or from the IceGridGUI. You can of course
also try to kill them. As long as one IceGrid registry is still
running, the client will continue to work.

Similarly, you can shutdown or kill one of the servers or nodes, and
the client will continue to work.

Finally, the registries and nodes are configured to redirect their
standard error and output to files in their data directories named
`stderr.txt` and `stdout.txt`. You can view these files using the
icegridadmin `registry show` or `node show` commands or with the
IceGridGUI. The standard error files contain the registry and
node traces.

[1]: https://doc.zeroc.com/ice/3.7/ice-services/icegrid/registry-replication
