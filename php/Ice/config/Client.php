<?php
// Copyright (c) ZeroC, Inc.

require_once 'Ice.php';
require_once 'Greeter.php';

// Load the contents of the config.client file into a Properties object.
$configFileProperties = Ice\createProperties();
$configFileProperties->load("config.client");

// Create a Properties object from the command line arguments and the config file properties; Ice.* properties and
// other reserved properties set in argv override the config file properties.
$properties = Ice\createProperties($argv, $configFileProperties);

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
// The communicator gets its properties from the properties object.
$communicator = Ice\initialize(new Ice\InitializationData($properties));

// We create a Greeter proxy using the value of the Greeter.Proxy property in config.client.
// It's null if the property is not set.
$greeter = VisitorCenter\GreeterPrxHelper::uncheckedCast($communicator->propertyToProxy("Greeter.Proxy"));

// Send a request to the remote object and get the response.
$greeting = $greeter->greet(get_current_user());

echo "$greeting\n";
