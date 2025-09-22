# Ice Greeter

This demo shows how to send a request and wait for a response using Ice for JavaScript. It includes a Node.js client
application.

## Building the demo

First install npm dependencies:

```shell
npm install
```

Then, to build the client application, run:

```shell
npm run build
```

## Running the demo

start the client application:

```shell
node client.js
```

> [!NOTE]
> Ice for JavaScript has limited server-side support. In this demo, we connect to a Greeter server implemented in a
> language with full server-side support, such as C++, C#, Java, Python, or Swift.
