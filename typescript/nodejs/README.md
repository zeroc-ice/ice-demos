# TypeScript Node.js Demos

## Overview

This directory contains TypeScript sample programs for various Ice components.
These examples are provided to get you started on using a particular Ice feature
or coding technique.

The following subdirectories contain sample programs:

- [Glacier2](./Glacier2) is a simpler chat client that uses the Glacier2 service.

- [Ice](./Ice) has several examples that illustrate fundamental Ice concepts.

- [Manual](./Manual) contains complete examples for some of the code snippets
in the [Ice manual][1].

Refer to the [C++11 demos](../../cpp11) for more examples that use Ice services
such as IceGrid and IceStorm.

## Build Instructions

### Prerequisites

The JavaScript sample programs require an installation of [Node.js][3].

### Building the Demos

#### Building the demos with the NPM packages

With Node.js in your PATH, go to the typescript/nodejs directory
and run the following command to install several dependencies:

```
npm install
```

This command builds the Slice-to-JavaScript compiler (`slice2js`) from
source and requires a C++ compiler such as Visual Studio 2015.

Now use gulp to build the demos:

```
npm run build
```

#### Building the demos with the Ice source distribution

If you want to build the demos using the Ice source distribution, you must set
the `ICE_HOME` environment variable with the path name of the Ice source
directory and pass the `--only=dev` argument to `npm install`:

```
export ICE_HOME=~/ice
npm install --only=dev
```

Now use gulp to build the demos:

```
npm run build
```

On Windows you need to set the platform and configuration in order to locate
slice2js. For example, if you have built C++ x64 Release mode, you can use the
following command to build:

```
npm run build -- --cppPlatform x64 --cppConfiguration Release
```

Alternatively, you can use the CPP_PLATFORM and CPP_CONFIGURATION environment
variables:

```
set CPP_PLATFORM=x64
set CPP_CONFIGURATION=Debug
npm run build
```

## Running the TypeScript Demos

Ice provides only client demos in TypeScript since Ice for JavaScript does not
support full server-side activities. In order to run a sample client, you must
first start its corresponding server from another Ice language mapping. If you
want to get started quickly, we recommend using the Python server. For example,
start the Python "hello" server:

```
cd python/Ice/hello
python Server.py
```

To run the client, open another terminal window, change to the corresponding
TypeScript demo directory, and run:

```
cd typescript/nodejs/Ice/hello
node Client
```

[1]: https://doc.zeroc.com/ice/4.0/introduction
