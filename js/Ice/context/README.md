# Ice Context

This demo illustrates how to use Ice request contexts.

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

First, start the server application.

Ice for JavaScript has limited server-side support. As a result, you need to start a Context server implemented in a
language that fully supports server-side functionality (C++, C#, Java, Python, or Swift), for example the [Python
Context server](../../../python/Ice/context): follow the instructions in its README to start this server.

In a separate terminal, start the client application:

```shell
node client.js
```
