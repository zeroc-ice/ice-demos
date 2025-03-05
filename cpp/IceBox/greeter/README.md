# IceBox Greeter

This demo shows how to create an [IceBox][1] service in C++.

You can build the client application and IceBox service with:

```shell
cmake -B build
cmake --build build --config Release
```

To run this demo, open two terminal windows. In the first window:

**Linux:**

```shell
LD_LIBRARY_PATH=./build icebox --IceBox.Service.Greeter="GreeterService:create --Ice.Trace.Dispatch"
```

**macOS:**

```shell
DYLD_LIBRARY_PATH=./build icebox --IceBox.Service.Greeter="GreeterService:create --Ice.Trace.Dispatch"
```

**Windows:**

```shell
set PATH=%PATH%;build\Release
icebox --IceBox.Service.Hello="GreeterService:create --Ice.Trace.Dispatch"
```

In the second window, run the client:

**Linux/macOS:**

```shell
./build/client
```

**Windows:**

```shell
build\Release\client
```

## Linux 32-bit

If you are using 32-bit binaries on a Linux 64-bit host, use `icebox32` instead of `icebox` to start the IceBox server.

[1]: https://doc.zeroc.com/ice/3.7/icebox
