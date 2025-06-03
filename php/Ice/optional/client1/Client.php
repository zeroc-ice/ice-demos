<?php
// Copyright (c) ZeroC, Inc.

require_once 'Ice.php';
require_once 'WeatherStation.php';

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
// This communicator is destroyed automatically at the end of the script.
$communicator = Ice\initialize($argv);

// Create a proxy to the weather station.
$weatherStation = ClearSky\WeatherStationPrxHelper::createProxy(
    $communicator,
    'weatherStation:tcp -h localhost -p 4061');

// Create an AtmosphericConditions object with random values.
$temperature = mt_rand(190, 230) / 10.0; // Temperature in degrees Celsius (19.0 to 23.0).
$humidity = mt_rand(450, 550) / 10.0; // Humidity in percentage (45.0 to 55.0).

$reading = new ClearSky\AtmosphericConditions($temperature, $humidity);

// Report this reading to the weather station.
$weatherStation->report('sensor-1', $reading);

echo "sensor v1: sent reading to weather station\n";
