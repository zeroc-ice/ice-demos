This demo illustrates how to write a QT GUI application which invokes
ordinary (twoway) operations, as well as how to make [oneway][1],
[datagram][2], [secure][3], and [batched][4] invocations.

## Building the demo on Linux and macOS

### Prerequisites

The demo requires [QT 5 SDK][7].

### Building the demo

```
qmake
make
```

## Building the demo on Windows

### Prerequisites

The project requires the [Qt 5 SDK][7], [Qt Visual Studio Tools][5] and [Ice Builder for Visual
Studio][6] extensions.

**When installing Qt 5 SDK ensure you select the MSVC 2015 64-bit component**

### Building the demo

Open `C++11 Qt demos.sln` Visual Studio solution and uset `Qt VS Tools` menu to setup
your Qt 5 Visual Studio 2015 Version, then use the `Change Solution's Qt Version`
option in the solution context menu to set the Qt version that will be used, once the Qt version
has been set you can build the solution.

## Running the demo

To run the demo, first start the hello server:

```
cd ../hello
server
```

In a separate window, start the qt client:

```
client
```

[1]: https://doc.zeroc.com/ice/4.0/client-side-features/oneway-invocations
[2]: https://doc.zeroc.com/ice/4.0/client-side-features/datagram-invocations
[4]: https://doc.zeroc.com/ice/4.0/client-side-features/batched-invocations
[3]: https://doc.zeroc.com/ice/4.0/ice-plugins/icessl
[5]: https://marketplace.visualstudio.com/items?itemName=TheQtCompany.QtVisualStudioTools2015
[6]: https://github.com/zeroc-ice/ice-builder-visualstudio
[7]: https://www.qt.io/
