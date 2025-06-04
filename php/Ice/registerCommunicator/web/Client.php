<?php
// Copyright (c) ZeroC, Inc.

require_once 'Ice.php';
require_once 'Greeter.php';

try {
    $name = isset($_POST['name']) ? $_POST['name'] : 'PHP';

    $communicator = Ice\find('Greeter');
    if ($communicator === null) {
        $communicator = Ice\initialize();
        $expires = 5; // minutes
        Ice\register($communicator, 'Greeter', $expires);
    }
    $greeter = VisitorCenter\GreeterPrxHelper::createProxy($communicator, 'greeter:tcp -h host.docker.internal -p 4061');

    $greeting = $greeter->greet($name);

    echo "<!DOCTYPE html><html><body>";
    echo "<h1>Greeting from Ice server:</h1>";
    echo "<p>" . htmlspecialchars($greeting) . "</p>";
    echo '<p><a href="index.html">Back</a></p>';
    echo "</body></html>";
} catch (Exception $e) {
    echo "<p>Error: " . htmlspecialchars($e->getMessage()) . "</p>";
}
