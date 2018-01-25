This demo illustrates how to write a QT GUI application which invokes
ordinary (twoway) operations, as well as how to make oneway, datagram,
secure, and batched invocations.

## Building the demo on Linux and macOS

### Prerequisites

The demo requires QT 5 package from https://www.qt.io/

### Building the demo

```
qmake -o qt.pro
Make
```

## Building the demo on Windows

### Prerequisites

The project requires the [Qt Visual Studio Tools][1] and [Ice Builder for Visual
Studio][2] extensions.

### Building the demo

Open `C++11 Qt demos.sln` Visual Studio solution and build the client project

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

[1]: https://marketplace.visualstudio.com/items?itemName=TheQtCompany.QtVisualStudioTools2015
[2]: https://github.com/zeroc-ice/ice-builder-visualstudio
