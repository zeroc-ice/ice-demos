<?php
// Copyright (c) ZeroC, Inc.

require_once 'Ice.php';
require_once 'Greeter.php';

// Create Ice properties from the contents of the config.client file in the current working directory.
$initData = new Ice\InitializationData();
$initData->properties = Ice\createProperties();
$initData->properties->load("config.client");

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
// The communicator gets its properties from initData.properties; Ice.* properties and other reserved properties
// set in argv override these properties.
$communicator = Ice\initialize($argv, $initData);

// We create a Greeter proxy using the value of the Greeter.Proxy property in config.client.
// It's null if the property is not set.
$greeter = VisitorCenter\GreeterPrxHelper::uncheckedCast($communicator->propertyToProxy("Greeter.Proxy"));

// Send a request to the remote object and get the response.
$greeting = $greeter->greet(get_current_user());

echo "$greeting\n";
