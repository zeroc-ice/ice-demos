# Ice Custom Error

The Custom error demo shows how to define an exception in Slice, and how to throw and catch this exception.

A Slice-defined exception should be seen as a custom error carried by the response instead of the expected return
value--there is naturally no throwing across the network.

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

Ice for JavaScript has limited server-side support. As a result, you need to start a Custom Error server implemented in a
language that fully supports server-side functionality, such as C++, C#, Java, Python, or Swift.

In a separate terminal, start the client application:

```shell
node client.js
```
