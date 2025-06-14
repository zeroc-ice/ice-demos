<?php
// Copyright (c) ZeroC, Inc.

require_once 'Ice.php';
require_once 'Greeter.php';

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
// This communicator is destroyed automatically at the end of the script.
$communicator = Ice\initialize($argv);

// GreeterPrxHelper is a class generated by the Slice compiler. We create a Greeter proxy from a communicator and a
// "stringified proxy" with the address of the target object.
// If you run the server on a different computer, replace localhost in the string below with the server's hostname
// or IP address.
$greeter = VisitorCenter\GreeterPrxHelper::createProxy($communicator, 'greeter:tcp -h localhost -p 4061');

$names = [get_current_user(), '', 'alice', 'bob', 'carol', 'dave', 'billy bob'];

foreach ($names as $name) {
    try {
        // Send a request to the remote object and get the response.
        $greeting = $greeter->greet($name);
        echo "$greeting\n";
    } catch (Ice\DispatchException $ex) {
        echo "Failed to create a greeting for '" . $name . "': DispatchException { message = '" . $ex->getMessage() .
            "', replyStatus = " . $ex->replyStatus . " }\n";
    } catch (VisitorCenter\GreeterException $ex) {
        echo "Failed to create a greeting for '" . $name . "': GreeterException { message = '" . $ex->getMessage() .
            "', error = " . $ex->error . " }\n";
    }
}
