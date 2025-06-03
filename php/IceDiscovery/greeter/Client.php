<?php
// Copyright (c) ZeroC, Inc.

require_once 'Ice.php';
require_once 'Greeter.php';

// Configure the communicator to load the IceDiscovery plug-in during initialization. This plug-in installs a default
// locator on the communicator.
$initData = new Ice\InitializationData();
$initData->properties = Ice\createProperties();
$initData->properties->setProperty('Ice.Plugin.IceDiscovery', '1');

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
// This communicator is destroyed automatically at the end of the script.
$communicator = Ice\initialize($argv, $initData);

// Create a proxy to the Greeter object hosted by the server(s). 'greeter' is a stringified proxy with no addressing
// information, also known as a well-known proxy. It's resolved by the default locator installed by the IceDiscovery
// plug-in.
$greeter = VisitorCenter\GreeterPrxHelper::createProxy($communicator, 'greeter');

// Send a request to the remote object and get the response.
$greeting = $greeter->greet(get_current_user());

echo "$greeting\n";
?>
