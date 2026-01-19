# TypeScript Browser Demos

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

With Node.js in your PATH, go to the typescript/browser directory and run the following command to install several
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

Ice provides only client demos in TypeScript since Ice for JavaScript does not support full server-side activities. The
browser clients require an HTTP server that supports WebSocket connections. For your convenience, this distribution
includes a Node.js HTTP server pre-configured for the demos. Open another terminal window and run:

```shell
npm run run
```

This will load <http://127.0.0.1:8080/index.html> in the default browser. From this page you can select the client that
you want to run.

Next, start the corresponding server from another Ice language mapping. Open a terminal window, change to the desired
server directory, and start the server. The client page provides a Readme link which might include additional
instructions for running the server.

To try a different client, terminate the server that you started earlier, change to the new server directory and start
the server for the new client.

### Secure WebSockets

The demos use secure WebSocket (WSS) when you access the demo pages through <https://127.0.0.1:9090/index.html>, and
non-secure WebSocket when you access them through <http://127.0.0.1:8080/index.html>. For demos that allow both secure
and non-secure WebSocket connections, there is a `protocol` checkbox that allows you to switch between secure and
non-secure WebSocket.

To run the demos over secure WebSocket (WSS), you need to trust the demo certificate from the [certs](../../certs)
directory.

[1]: https://doc.zeroc.com/ice/3.7/introduction
[2]: http://nodejs.org
