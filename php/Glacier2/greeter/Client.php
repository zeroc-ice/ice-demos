<?php
// Copyright (c) ZeroC, Inc.

require_once 'Ice.php';
require_once 'Glacier2.php';
require_once 'Greeter.php';

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
// This communicator is destroyed automatically at the end of the script.
$communicator = Ice\initialize($argv);

// Create a proxy to the Glacier2 router. The addressing information (transport, host and port number) is derived
//  from the value of Glacier2.Client.Endpoints in the Glacier2 router configuration file.
$router = Glacier2\RouterPrxHelper::createProxy($communicator, 'Glacier2/router:tcp -h localhost -p 4063');

// Retrieve my username.
$username = get_current_user();

// Create a session with the Glacier2 router. In this demo, the Glacier2 router is configured to accept any
// username/password combination. This call establishes a network connection to the Glacier2 router; the lifetime of
// the session is the same as the lifetime of the connection.
$session = $router->createSession($username, 'password');

// The proxy returned by createSession is null because we did not configure a SessionManager on the Glacier2 router.
assert($session === null);

// Create a proxy to the Greeter object in the server behind the Glacier2 router. Typically, the client cannot
// connect directly to this server because it's on an unreachable network.
$greeter = VisitorCenter\GreeterPrxHelper::createProxy($communicator, 'greeter:tcp -h localhost -p 4061');

// Configure the proxy to route requests using the Glacier2 router.
$greeter = $greeter->ice_router($router);

// Send a request to the remote object and get the response.
$greeting = $greeter->greet($username);
echo "$greeting\n";

// Send a second request to observe the effect in the Glacier2 router log.
$greeting = $greeter->greet('alice');
echo "$greeting\n";
