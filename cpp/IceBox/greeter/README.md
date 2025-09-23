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
cmake -B build -S . -G Ninja
cmake --build build
```

## Running the demo

To run this demo, open two terminal windows. In the first window:

**Linux/macOS:**

```shell
icebox --IceBox.Service.Greeter="./build/GreeterService:create --Ice.Trace.Dispatch"
```

**Windows:**

```shell
build\icebox --IceBox.Service.Greeter="build\GreeterService:create --Ice.Trace.Dispatch"
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

[Ice for C++ installation]: https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#ice-for-c
[Ice service installation]: https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#ice-services
