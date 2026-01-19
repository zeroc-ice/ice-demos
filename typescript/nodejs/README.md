# TypeScript Node.js Demos

## Overview

This directory contains TypeScript sample programs for various Ice components. These examples are provided to get you
started on using a particular Ice feature or coding technique.

The following subdirectories contain sample programs:

- [Glacier2](./Glacier2) is a simpler chat client that uses the Glacier2 service.

- [Ice](./Ice) has several examples that illustrate fundamental Ice concepts.

- [Manual](./Manual) contains complete examples for some of the code snippets in the [Ice manual][1].

Refer to the [C++11 demos](../../cpp11) for more examples that use Ice services such as IceGrid and IceStorm.

## Build Instructions

### Prerequisites

The TypeScript sample programs require an installation of [Node.js][2].

### Building the Demos

#### Building the demos with the NPM packages

With Node.js in your PATH, go to the typescript/nodejs directory and run the following command to install several
dependencies:

```shell
npm install
```

This command builds the Slice-to-JavaScript compiler (`slice2js`) from source and requires a C++ compiler such as
Visual Studio 2015.

Now use gulp to build the demos:

```shell
npm run build
```

#### Building the demos with nightly packages

You can build the demos using the latest Ice nightly packages from the ZeroC nightly repository. First, run the setup
script from the repository root:

```shell
python scripts/setup-nightly.py --channel 3.7
```

Then install dependencies and build:

```shell
npm install
npm run build
```

To reset back to the release packages:

```shell
python scripts/setup-nightly.py --reset
```

## Running the TypeScript Demos

Ice provides only client demos in TypeScript since Ice for JavaScript does not support full server-side activities. In
order to run a sample client, you must first start its corresponding server from another Ice language mapping. If you
want to get started quickly, we recommend using the Python server. For example, start the Python "hello" server:

```shell
cd python/Ice/hello
python Server.py
```

To run the client, open another terminal window, change to the corresponding TypeScript demo directory, and run:

```shell
cd typescript/nodejs/Ice/hello
node Client
```

[1]: https://doc.zeroc.com/ice/3.7/introduction
[2]: http://nodejs.org
