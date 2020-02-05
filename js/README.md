# JavaScript Demos

## Overview

This directory contains JavaScript sample programs for various Ice components.
These examples are provided to get you started on using a particular Ice feature
or coding technique.

The following subdirectories contain sample programs:

- [Chat](./Chat) is a JavaScript client for the ZeroC [Chat Demo][1].

- [Glacier2](./Glacier2) is a simpler chat client that uses the Glacier2 service.

- [Ice](./Ice) has several examples that illustrate fundamental Ice concepts.

- [Manual](./Manual) contains complete examples for some of the code snippets
in the [Ice manual][2].

Refer to the [C++11 demos](../cpp11) for more examples that use Ice services
such as IceGrid and IceStorm.

## Build Instructions

### Prerequisites

The JavaScript sample programs require an installation of [Node.js][3].

### Building the Demos

#### Building the demos with the NPM packages

With Node.js in your PATH, go to the [js directory](../js) and run the
following command to install several dependencies:

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

## Running the Node.js JavaScript Demos

Ice provides only client demos in JavaScript since Ice for JavaScript does not
support full server-side activities. In order to run a sample client, you must
first start its corresponding server from another Ice language mapping. If you
want to get started quickly, we recommend using the Python server. For example,
start the Python "hello" server:

```
cd python/Ice/hello
python Server.py
```

To run the client, open another terminal window, change to the corresponding
JavaScript demo directory, and run:

```
cd js/Ice/hello
node Client
```

The JavaScript demos use `async`, `await` and other features introduced
in ES2017. For compatibility with JavaScript interpreters that only include
support for ES5, the build creates `es5` versions of the demos using the Babel
JavaScript compiler. The es5 versions can be run:

```
cd js/Ice/hello/es5
node Client
```

## Running the Browser JavaScript Demos

Ice provides only client demos in JavaScript since Ice for JavaScript does not
support full server-side activities. The browser clients require an HTTP server
that supports WebSocket connections. For your convenience, this distribution
includes a Node.js HTTP server pre-configured for the demos. Open another
terminal window and run:

```
npm run run
```

This will load http://127.0.0.1:8080/index.html in the default browser. From
this page you can select the client that you want to run.

Next, start the corresponding server from another Ice language mapping. Open a
terminal window, change to the desired server directory, and start the server.
The client page provides a Readme link which might include additional
instructions for running the server.

To try a different client, terminate the server that you started earlier, change
to the new server directory and start the server for the new client.

#### Secure WebSockets

The demos use secure WebSocket (WSS) when you access the demo pages through
https://127.0.0.1:9090/index.html, and non-secure WebSocket when you access
them through http://127.0.0.1:8080/index.html. For demos that allow both
secure and non-secure WebSocket connections, there is a `protocol` checkbox
that allows you to switch between secure and non-secure WebSocket.

##### Self-Signed Certificate

The secure WebSocket (WSS) connections in these demos rely on a self-signed
certificate, `cacert`, provided in the certs(../certs) directory.

To successfully run the demos with WSS, additional action may be necessary
depending on the browser you're using:

- Chrome
   You'll be presented with a warning about the site's security certificate.
   Click the `Proceed anyway` button to temporarily accept the certificate.

- Firefox
   You'll see a warning saying "This Connection is Untrusted". Open Firefox's
   Preferences or Options dialog, click on the Advanced section, select the
   Certificates tab and click on the "View Certificates..." button. In the
   Authorities tab, click the "Import..." button, navigate to the `cacert.pem`
   file, and add it as a certificate authority (CA) for trusting web sites.
   After closing the dialogs, reload the test page to continue. You should
   uninstall this certificate after running the demos.

- Internet Explorer and Microsoft Edge
   Run the management console (mmc.exe) and add the Certificates snap-in for
   the computer account. Then select Console Root > Certificates (Local Computer)
   \> Trusted Root Certificate Authorities. In the Action menu, choose All Tasks
   and Import. Navigate to the `cacert.der` file and import it into the Trusted
   Root Certificate Authorities. Reload the test page to continue. You should
   uninstall this certificate after running the demos.

To use WSS on iOS and Android it may be required (depending on browser and
platform) that the server certificate's common name matches the computer hosting
the demos, and that the demo certificate authority be installed on your device.

First you'll need to generate new certificates to match the IP address or DNS
name of the computer hosting the demos:

```
certs/makedemocerts.py --ip [IP address] --dns [DNS name] [--use-dns]
```

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

[1]: https://doc.zeroc.com/technical-articles/general-topics/chat-demo
[2]: https://doc.zeroc.com/ice/4.0/introduction
[3]: http://nodejs.org
