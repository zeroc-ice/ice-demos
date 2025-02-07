# IceGrid Allocate

This demo shows how to [allocate objects][1] using the IceGrid registry.

To build the demo run:

```shell
cmake -B build
cmake --build build --config Release
```

To run the demo, first open a new terminal window for the IceGrid service:

Add the server build directory to the PATH environment variable:

**Linux/macOS:**

```shell
export PATH=$(pwd)/build:$PATH
```

**Windows:**

```shell
set PATH=%CD%\\build\\Release;%PATH%
```

Now start the IceGrid service:

```shell
icegridnode --Ice.Config=config.grid
```

This demo contains two application descriptor files. The first
descriptor, application-single.xml, contains a single server and
object. This object is allocated by the client using the
allocateObjectById operation. Only one client can access this object
at a time. All other clients will hang until the object is released.
Use the following command to deploy this descriptor:

```shell
icegridadmin --Ice.Config=config.icegridadmin -e "application add application-single.xml"
```

The second descriptor, application-multiple.xml, contains two servers,
each with an object. The clients retrieve these objects using
allocateObjectByType. Since there are two objects available, two
clients can get access simultaneously. Additional clients will hang
until one of the clients with an allocated object releases it. Use the
following command to deploy this descriptor:

```shell
icegridadmin --Ice.Config=config.icegridadmin -e "application add application-multiple.xml"
```

To run the client type:

**Linux/macOS:**

```shell
./build/client
```

**Windows:**

```shell
build\Release\client
```

If you have already deployed the application, you can update it to try
a different set of descriptors, for example:

```shell
icegridadmin --Ice.Config=config.icegridadmin -e \ "application update application-multiple.xml"
```

Messages will be displayed in the IceGrid service window.

[1]: https://doc.zeroc.com/ice/3.7/ice-services/icegrid/resource-allocation-using-icegrid-sessions
