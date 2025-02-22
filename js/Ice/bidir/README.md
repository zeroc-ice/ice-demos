# Bidir

The Bidir demo illustrates how to send requests "the other way around", from a server to a client, by reusing the
connection established by the client to the server.

Ice for JavaScript has limited server-side support ([see documentation][1]). As a result, you need to start a Greeter
server implemented in a language that fully supports server-side functionality, such as Python, Java, or C#.

## Installation

Before building the client, install the dependencies:

```shell
npm install
```

## Building the Client

Once the dependencies are installed, build the client application with:

```shell
npm run build
```

## Running the Client

After building, run the client application with:

```shell
node client.js
```

[1]: https://doc.zeroc.com/ice/3.7/language-mappings/javascript-mapping
