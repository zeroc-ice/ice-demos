% Copyright (c) ZeroC, Inc.

function client(args)
    arguments (Repeating)
        args (1, :) char
    end

    % Load the Ice library if it is not already loaded.
    if ~libisloaded('ice')
        loadlibrary('ice', @iceproto);
    end

    % Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
    communicator = Ice.Communicator(args);

    % Destroy the communicator when the function exits.
    cleanup = onCleanup(@() communicator.destroy());

    % Create a proxy to the weather station.
    weatherStation = clearsky.WeatherStationPrx(communicator, 'weatherStation:tcp -h localhost -p 4061');

    % Create an AtmosphericConditions object with random values.
    temperature = randi([190, 230]) / 10; % Temperature in degrees Celsius (19.0 to 23.0).
    humidity = randi([450, 550]) / 10; % Humidity in percentage (45.0 to 55.0).
    pressure = randi([10000, 10500]) / 10; % Pressure in millibars (1,000 to 1,050).

    reading = clearsky.AtmosphericConditions(temperature, humidity, pressure);

    % Report this reading to the weather station.
    weatherStation.report('sensor-2', reading);

    fprintf('sensor v2: sent reading to weather station\n');
end
