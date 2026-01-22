# Ice interceptor

This demo shows how to use [Dispatch Interceptors] to implement a token based authentication system for securing
operations on a remote object.

To run the demo, first start the server:

```shell
.build/debug/Server
```

In a separate window, start the client:

```shell
.build/debug/Client
```

The demo models a remote controlled thermostat with operations for getting and setting its temperature, and for shutting
down the thermostat server. The demo is configured so that any client can get the thermostat's temperature, but only
clients authenticated with the server can set it, or shut it down. To do this, the server has two objects: one for
controlling the thermostat, and another for handling authentication requests.

The authenticator object only has one exposed operation, which issues access tokens. After retrieving a token, the
client adds it to the communicator's [Request Context], so it's transmitted alongside any requests it makes to the
server.

Internally, the server keeps a list of valid access tokens, and uses a [Dispatch Interceptor][Dispatch Interceptors] to intercept incoming
requests to the thermostat object. The interceptor ensures the request has a valid access token before letting it invoke
any secured operations.

[Dispatch Interceptors]: https://doc.zeroc.com/ice/3.7/server-side-features/dispatch-interceptors
[Request Context]: https://doc.zeroc.com/ice/3.7/client-side-features/request-contexts
