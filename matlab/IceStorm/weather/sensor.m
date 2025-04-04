% Copyright (c) ZeroC, Inc.

function sensor(args)
    % The Slice module ClearSky in WeatherStation.ice maps a MATLAB package with the same name.
    import ClearSky.*

    if nargin == 0
        args = {};
    end

    % Load the Ice library if it is not already loaded.
    if ~libisloaded('ice')
        loadlibrary('ice', @iceproto);
    end

    % Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
    communicator = Ice.initialize(args);

    % Destroy the communicator when the function exits.
    cleanup = onCleanup(@() communicator.destroy());

    % Create a proxy to the IceStorm topic manager.
    topicManager = IceStorm.TopicManagerPrx(communicator, 'ClearSky/TopicManager:tcp -p 4061 -h localhost');

    % Retrieve a proxy to the 'weather' topic: we first create a topic with the given name (in case we are the first),
    % and then retrieve the proxy if the topic was already created.
    topicName = 'weather';

    try
        topic = topicManager.create(topicName);
    catch ex
        if isa(ex, 'IceStorm.TopicExists')
            topic = topicManager.retrieve(topicName);
        else
            rethrow(ex);
        end
    end

    % The proxy returned by create and retrieve is never null.
    assert(~isempty(topic));

    % Create a WeatherStation proxy using the publisher proxy of the topic. The proxy returned by getPublisher is
    % never null.
    weatherStation = WeatherStationPrx.uncheckedCast(topic.getPublisher());

    % The proxy returned by IceStorm is a two-way proxy. We can convert it into a one-way proxy if we don't need
    % acknowledgments from IceStorm.
    assert(weatherStation.ice_isTwoway());

    % Generate a sensor ID for this weather sensor.
    sensorId = sprintf('sensor-%d', randi(1000));

    % Send a reading every second to the weather station(s) via IceStorm. We keep sending until the user presses Ctrl+C.
    fprintf('%s reporting. Press Ctrl+C to stop...\n', sensorId);

    while true
        % Generate a random temperature and humidity reading.
        temperature = randi([190, 230]) / 10; % Temperature in degrees Celsius (19.0 to 23.0).
        humidity = randi([450, 550]) / 10; % Humidity in percentage (45.0 to 55.0).

        % Create an AtmosphericConditions object with the generated values.
        reading = AtmosphericConditions(temperature, humidity);

        % Send the reading to the weather station(s) via IceStorm.
        timeStamp = datetime('now', 'Format', 'HH:mm:ss');
        weatherStation.report(sensorId, char(timeStamp), reading);

        % Wait for one second before sending the next reading.
        pause(1);
    end
end
