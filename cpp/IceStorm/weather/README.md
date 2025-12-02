# IceStorm Weather

This demo shows how to use IceStorm to create a simple pub-sub application. In this demo, one or more weather sensors
report the local temperature and humidity to one or more weather stations via IceStorm. The sensors are the publishers
while the weather stations are the subscribers.

```mermaid
flowchart LR
    p1[Sensor #374] --report--> icestorm[IceStorm service<br>Hosts topic 'weather']
    p2[Sensor #789] --report--> icestorm
    icestorm --report--> s1[Station #1]
    icestorm --report--> s2[Station #2]
    icestorm --report--> s3[Station #3]
```

## Ice prerequisites

- Install the C++ dev kit.
  - Linux and macOS: see [Ice for C++ installation].
  - Windows: the cmake build downloads and installs the C++ dev kit automatically.
- Install IceStorm. See [Ice service installation].

## Building the demo

You can build the weather sensor and weather station applications with:

```shell
cmake -B build -S . -G Ninja
cmake --build build
```

## Running the demo

1. Start the IceStorm service in its own terminal:

   ```shell
   icebox --IceBox.Service.IceStorm="IceStormService,38a0:createIceStorm --Ice.Config=icestorm.conf"
   ```

2. Run one or more sensors and weather stations, each in its own terminal. You can start them in any order.

   To start a weather station:

    **Linux/macOS:**

    ```shell
    ./build/station
    ```

    **Windows:**

    ```shell
    build\station
    ```

    To start a weather sensor:

    **Linux/macOS:**

    ```shell
    ./build/sensor
    ```

    **Windows:**

    ```shell
    build\sensor
    ```

[Ice for C++ installation]: https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#ice-for-c
[Ice service installation]: https://github.com/zeroc-ice/ice/blob/main/NIGHTLY.md#ice-services
