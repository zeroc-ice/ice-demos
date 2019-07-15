This demo illustrates the use of [Dispatch Interceptors][1] to secure operations on a remote object so that they require client authentication to be accessed.

To run the demo, first start the server:

```
java -jar build/libs/server.jar
```

In a separate window, start the client:

```
java -jar build/libs/client.jar
```

The demo models a remote controlled thermostat with operations for getting and setting the temperature. It's configured so that any client can get the temperature, but it must be authenticated with the server before it can set the temperature. This demo uses an OAuth provider alongside the Ice server that clients can authenticate with, sending a username and password in exchange for an access token the client can use instead of login credentials. The client includes this access token in it's [request context][2] so it is transmitted alongside any requests it makes to the server.

Since the server doesn't actually authenticate login credentials, any username or password can be used with the demo.

[1]: https://doc.zeroc.com/ice/3.7/server-side-features/dispatch-interceptors
[2]: https://doc.zeroc.com/ice/3.7/client-side-features/request-contexts
