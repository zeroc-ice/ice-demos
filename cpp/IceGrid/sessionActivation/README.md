# IceGrid Session Activation

This demo demonstrates the use of IceGrid's [session activation mode][1]
where the server is activated on demand once it is allocated by the client
and deactivated when the client releases the server.

To build the demo run:

```shell
cmake -B build
cmake --build build --config Release
```

To run the demo, first open a new terminal window for the IceGrid service.

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

In a separate window deploy the application as described in the file `application.xml`:

```shell
icegridadmin --Ice.Config=config.admin -e "application add application.xml"
```

Next, start the client

```shell
client
```

Messages will be displayed in the IceGrid service window.

[1]: https://doc.zeroc.com/ice/3.7/ice-services/icegrid/resource-allocation-using-icegrid-sessions
