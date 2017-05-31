# JavaScript Demos

## Overview

This directory contains JavaScript sample programs for various Ice components. These
examples are provided to get you started on using a particular Ice feature or coding
technique.

The following subdirectories contain sample programs:

- [Chat](./Chat) is a JavaScript client for the ZeroC [Chat Demo][1].

- [Glacier2](./Glacier2) is a simpler chat client that uses the Glacier2 service.

- [Ice](./Ice) has several examples that illustrate fundamental Ice concepts.

Refer to the [C++11 demos](../cpp11) for more examples that use Ice services
such as IceGrid and IceStorm.

## Build Instructions

### Prerequisites

The JavaScript sample programs require an installation of [Node.js][2].

### Building the Demos

With Node.js in your PATH, go to this directory (ice-demos/js) and run the
following command to install several dependencies:

    npm install

This command builds the Slice-to-JavaScript compiler (`slice2js`) from
source and requires a supported C++ compiler.

If you want to build the demos using the Ice source distribution, you must set
`ICE_HOME` environment variable with the path name of the Ice source directory
and pass `--only=dev` argument to `npm install`:

    > export ICE_HOME=~/ice
    > npm install --only=dev

Now use gulp to build the demos:

    npm run build

On Windows you need to set the platform and configuration in order to locate
slice2js, for example if you have build C++ x64 Release mode you can use
the following command to build

    > npm run build -- --cppPlatform x64 --cppConfiguration Release

alternativelly you can use the CPP_PLATFORM and CPP_CONFIGURATION enviroment
variables:

    > set CPP_PLATFORM=x64
    > set CPP_CONFIGURATION=Debug
    > npm run build

## Running the Node.js JavaScript Demos

Ice provides only client demos in JavaScript since Ice for JavaScript does not support
full server-side activities. In order to run a sample client, you must first start its
corresponding server from another Ice language mapping. For example, start the python
"hello" server:

    > cd python/Ice/hello
    > python Server.py

To run the client, open another terminal window, change to the corresponding
JavaScript demo directory, and run:

    > cd js/Ice/hello
    > node Client

## Running the Browser JavaScript Demos

Ice provides only client demos in JavaScript since Ice for JavaScript does not support
full server-side activities. The browser clients require an HTTP server that supports
WebSocket connections. For your convenience, this distribution includes a Node.js
HTTP server pre-configured for the demos. Open another terminal window and run:

    > npm run run

This will load http://127.0.0.1:8080/index.html in the default browser. From this page
you can select the client that you want to run.

Next, start the corresponding server from another Ice language mapping. Open a
terminal window, change to the desired server directory, and start the server. The
client page provides a README link which might include additional instructions for
running the server; please refer to the README of each client before starting the
server.

To try a different client, terminate the server that you started earlier, change
to the new server directory and start the new server.

#### Secure WebSockets on iOS and Android

To use WSS on iOS and Android it may be required (depending on browser and
platform) that the server certificate's common name matches the computer hosting
the demos, and that the demo certificate authority be installed on your device.

First you'll need to generate new certificates to match the IP address or DNS
name of the computer hosting the demos:

    > certs/makedemocerts.py --ip [IP address] --dns [DNS name] [--use-dns]

The option `--use-dns` specifies to use the DNS name instead of the IP address
for the certificate common name. You can use either the IP address or DNS name
for the common name, the only requirement is that it matches the value used to
connect to the server.

Next you must install the certificate authority on your device. The simplest way
is to email the CA certificate (`certs/cacert.pem`) to yourself and then follow
the instructions below to install the certificate on your device.

Once installed, you connect to the server using the same IP address or DNS name
used to create the certificates.

##### Installing Certificates on iOS

Open the certificate (`cacert.pem`) from the device's email client. You
will be prompted to create a configuration profile containing this certificate.

##### Installing Certificates on Android

Download the certificate (`cacert.pem`) to the device from an email client.
Next go to _Settings -> Security -> Install from storage_, and choose
`cacert.pem`. Enter a name and press OK.

[1]: https://zeroc.com/chat/index.html
[2]: http://nodejs.org
