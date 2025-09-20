# IceBox Greeter

This demo shows how to create an IceBox service in C++.

## Ice prerequisites

- Install the C++ dev kit.
  - Linux and macOS: see [Ice for C++ installation].
  - Windows: the cmake build downloads and installs the C++ dev kit automatically.
- Install IceBox.
  - Linux and macOS: see [Ice service installation].
  - Windows: IceBox is included in the C++ dev kit on Windows.

## Building the demo

You can build the client application and IceBox service with:

```shell
cmake -B build
cmake --build build --config Release
```

## Running the demo

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

[Ice for C++ installation]: https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#ice-for-c
[Ice service installation]: https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#ice-services
