<?php
// Copyright (c) ZeroC, Inc.

require_once 'Ice.php';
require_once 'IceStorm.php';
require_once 'WeatherStation.php';

// Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
// This communicator is destroyed automatically at the end of the script.
$communicator = Ice\initialize($argv);

// Create a proxy to the IceStorm topic manager.
$topicManager = IceStorm\TopicManagerPrxHelper::createProxy(
    $communicator,
    'ClearSky/TopicManager:tcp -p 4061 -h localhost');

// Retrieve a proxy to the 'weather' topic: we first create a topic with the given name (in case we are the first),
// and then retrieve the proxy if the topic was already created.
$topicName = 'weather';

try {
    $topic = $topicManager->create($topicName);
}
catch (IceStorm\TopicExists $ex) {
    $topic = $topicManager->retrieve($topicName);
}

// The proxy returned by create and retrieve is never null.
assert($topic !== null);

// Create a WeatherStation proxy using the publisher proxy of the topic. The proxy returned by getPublisher is never
// null.
$weatherStation = ClearSky\WeatherStationPrxHelper::uncheckedCast($topic->getPublisher());

// The proxy returned by IceStorm is a two-way proxy. We can convert it into a one-way proxy if we don't need
// acknowledgments from IceStorm.
assert($weatherStation->ice_isTwoway());

// Generate a sensor ID for this weather sensor.
$sensorId = "sensor-" . mt_rand(1, 1000);

// Send a reading every second to the weather station(s) via IceStorm. We keep sending until the user presses Ctrl+C.
echo "$sensorId reporting. Press Ctrl+C to stop...\n";

while (true) {
    // Generate a random temperature and humidity reading.
    $temperature = mt_rand(190, 230) / 10.0;
    $humidity = mt_rand(450, 550) / 10.0;

    // Create an AtmosphericConditions object with the generated values.
    $reading = new ClearSky\AtmosphericConditions($temperature, $humidity);

    // Send the reading to the weather station(s) via IceStorm.
    $timeStamp = date('H:i:s');
    $weatherStation->report($sensorId, $timeStamp, $reading);

    // Wait for one second before sending the next reading.
    sleep(1);
}
