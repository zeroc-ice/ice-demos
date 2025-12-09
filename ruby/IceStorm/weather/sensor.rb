#!/usr/bin/env ruby
# Copyright (c) ZeroC, Inc.

require 'date'
require 'Ice'
require 'IceStorm'

# The Slice module ClearSky in WeatherStation.ice maps a Ruby module with the same name.
require_relative 'WeatherStation.rb'

# Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
Ice.initialize(ARGV) do |communicator|

    # Create a proxy to the IceStorm topic manager.
    topicManager = IceStorm::TopicManagerPrx.new(communicator, "ClearSky/TopicManager:tcp -p 4061 -h localhost")

    # Ask the topic manager to create or retrieve the "weather" topic and return the corresponding proxy.
    topic = topicManager.createOrRetrieve('weather')

    # Create a WeatherStation proxy using the publisher proxy of the topic. The proxy returned by getPublisher is
    # never nil.
    weatherStation = ClearSky::WeatherStationPrx::uncheckedCast(topic.getPublisher())

    # Generate a sensor ID for this weather sensor.
    sensorId = "sensor-" + rand(1000).to_s

    # Send a reading every second to the weather station(s) via IceStorm. We keep sending until the user presses Ctrl+C.
    puts sensorId + " reporting. Press Ctrl+C to stop..."

    cancel = false

    Signal.trap("INT") do
        cancel = true
        puts ""
        puts "Stopping..."
    end

    while !cancel
        # Generate a random temperature and humidity reading.
        temperature = (190.0 + rand(40)) / 10.0; # Temperature in degrees Celsius (19.0 to 23.0).
        humidity = (450.0 + rand(100)) / 10.0; # Humidity in percentage (45.0 to 55.0).

        # Create an AtmosphericConditions object with the generated values.
        reading = ClearSky::AtmosphericConditions.new(temperature, humidity)

        # Send the reading to the weather station(s) via IceStorm.
        timeStamp = DateTime.now
        weatherStation.report(sensorId, timeStamp.to_s, reading)

        # Wait for one second before sending the next reading.
        sleep(1)
    end

end
