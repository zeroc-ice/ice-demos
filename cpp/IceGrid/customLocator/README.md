# IceGrid Custom Locator

This demo shows how to implement a [custom locator][1] that delegates
to the [IceGrid][2] registry.

To build the demo run:

```shell
cmake -B build
cmake --build build --config Release
```

To run the demo, first open a new terminal window for the IceGrid service:

Add the server build directory to the PATH environment variable:

**Linux/macOS:**

```shell
export PATH=$PWD/build:$PATH
```

**Windows:**

```shell
set PATH=%CD%\\build\\Release;%PATH%
```

Now start the IceGrid service:

```shell
icegridnode --Ice.Config=config.grid
```

In a separate window start the custom locator:

**Linux/macOS:**

```shell
./build/locator
```

**Windows:**

```shell
build\Release\locator
```

Now in another window deploy the application:

```shell
icegridadmin --Ice.Config=config.admin -e "application add application.xml"
```

The custom locator included in this demo only allows clients that set a
specific value in the Locator proxy context to obtain a proxy to the server
object.

To run the client without setting this value:

**Linux/macOS:**

```shell
./build/client
```

**Windows:**

```shell
build\Release\client
```

To run the client with the context set properly:

**Linux/macOS:**

```shell
./build/client --context
```

**Windows:**

```shell
build\Release\client --context
```

[1]: https://doc.zeroc.com/ice/3.7/client-server-features/locators
[2]: https://doc.zeroc.com/ice/3.7/ice-services/icegrid
