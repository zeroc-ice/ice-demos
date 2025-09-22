# Ice Bidir

The Bidir demo illustrates how to send requests "the other way around", from a server to a client, by reusing the
connection established by the client to the server.

## Building the demo

First instal npm dependencies:

```shell
npm install
```

Then, to build the client application, run:

```shell
npm run build
```

## Running the demo

First, start the server application.

Ice for JavaScript has limited server-side support. As a result, you need to start a Bidir server implemented in a
language that fully supports server-side functionality, such as Python, Java, or C#.

In a separate terminal, start the client application:

```shell
node client.js
```
