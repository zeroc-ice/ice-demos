<?php
// Copyright (c) ZeroC, Inc.

require_once 'Ice.php';
require_once 'Greeter.php';

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
// This communicator is destroyed automatically at the end of the script.
$communicator = Ice\initialize($argv);

// Set the default locator of the new communicator. It's the address of the Locator hosted by our IceGrid registry.
// You can also set this proxy with the Ice.Default.Locator property.
$communicator->setDefaultLocator(
    Ice\LocatorPrxHelper::createProxy($communicator, 'IceGrid/Locator:tcp -h localhost -p 4061'));

// Create a proxy to the Greeter object hosted by the server. 'greeter' is a stringified proxy with no addressing
// information, also known as a well-known proxy. It's specified by the <object> element in the IceGrid XML file.
// The IceGrid registry resolves this well-known proxy and returns the actual address (endpoint) of the server to
// this client.
$greeter = VisitorCenter\GreeterPrxHelper::createProxy($communicator, 'greeter');

// Send a request to the remote object and get the response.
$greeting = $greeter->greet(get_current_user());
echo "$greeting\n";

// Send another request to the remote object. With the default configuration we use for this client, this request
// reuses the connection and reaches the same server, even when we have multiple replicated servers.
$greeting = $greeter->greet('alice');
echo "$greeting\n";
