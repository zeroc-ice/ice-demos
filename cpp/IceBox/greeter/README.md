# IceBox Greeter

This demo shows how to create an IceBox service in C++.

You can build the client application and IceBox service with:

```shell
cmake -B build -S . -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

To run this demo, open two terminal windows. In the first window:

**Linux/macOS:**

```shell
icebox --IceBox.Service.Greeter="./build/GreeterService:create --Ice.Trace.Dispatch"
```

**Windows:**

```shell
icebox --IceBox.Service.Hello="build\GreeterService:create --Ice.Trace.Dispatch"
```

In the second window, run the client:

**Linux/macOS:**

```shell
./build/client
```

**Windows:**

```shell
build\client
```

## Linux 32-bit

If you are using 32-bit binaries on a Linux 64-bit host, use `icebox32` instead of `icebox` to start the IceBox server.
