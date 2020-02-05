# MATLAB Demos

## Overview

This directory contains MATLAB sample programs for various Ice components.
These examples are provided to get you started on using a particular Ice
feature or coding technique.

The following subdirectories contain sample programs:

- [Ice](./Ice) has several examples that illustrate fundamental Ice concepts.

- [IceDiscovery](./IceDiscovery) illustrates how to use the IceDiscovery plug-in.

- [Manual](./Manual) contains complete examples for some of the code snippets in the [Ice Manual][1].

## Build Instructions

Open MATLAB, and change to the [matlab](../matlab) demo directory
using the MATLAB console:

```
cd matlab
```

To build the demos run the `build.m` script:

```
build
```

To clean the demos run the `clean.m` script:

```
clean
```

### Building the demos with the Ice source distribution

Add the following directories to your MATLAB search path:

 - `<IceHome>\matlab\lib`
 - `<IceHome>\matlab\lib\generated`
 - `<IceHome>\matlab\lib\x64\Release`

[1]: https://doc.zeroc.com/ice/4.0/introduction
