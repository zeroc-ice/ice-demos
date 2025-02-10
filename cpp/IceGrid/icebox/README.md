# IceGrid IceBox

This demo shows a very simple [IceGrid deployment][1], with a single
[IceBox][2] server hosting a number of IceBox services.

The demo also demonstrates how to use an environment variable in a
server descriptor.

In `application.xml`, `<env>LANG=en</env>` defines an environment variable
named `LANG` with value `en`. The Hello object implementation HelloI
reads the value of `LANG` and shows the message in the corresponding
language; you can try to update the value to `fr`, `de`, `es` or `it`.

To build the demo run:

```shell
cmake -B build
cmake --build build --config Release
```

## Setup

To run the demo, first open a new terminal window for the IceGrid service:

Add the server build directory to the PATH environment variable:

**Linux:**

```shell
export PATH=$PWD/build:$PATH
export LD_LIBRARY_PATH=$PWD/build
```

**macOS:**

```shell
export PATH=$PWD/build:$PATH
export DYLD_LIBRARY_PATH=$PWD/build
```

**Windows:**

```shell
set PATH=%CD%\\build\\Release;%PATH%
```

Now start the IceGrid service:

```shell
icegridnode --Ice.Config=config.grid
```

Deploy the `HelloSimpsons` application (in file `application.xml`) with
the IceGridGUI (see below). If you prefer to use the command-line utility, use:

```shell
icegridadmin --Ice.Config=config.admin -e "application add application.xml"
```

If you are using 32-bit binaries on a Linux 64-bit host, first edit
`application.xml` and replace `exe="icebox"` by `exe="icebox32"`.

## Running the Client

To run the client type:

**Linux/macOS:**

```shell
./build/client
```

**Windows:**

```shell
build\Release\client
```

The client simply calls `sayHello` on the replicated `hello` object.

## Stopping and restarting IceBox services

You can use the IceGrid GUI or the `icegridadmin` command-line utility to stop and
restart IceBox services. The commands below show how to manipulate
the 'Lisa' service:

```shell
icegridadmin --Ice.Config=config.grid
>>> service stop IceBox Lisa
>>> service start IceBox Lisa
```

[1]: https://doc.zeroc.com/ice/3.7/ice-services/icegrid/using-icegrid-deployment
[2]: https://doc.zeroc.com/ice/3.7/icebox
