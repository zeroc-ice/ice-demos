% Copyright (c) ZeroC, Inc.

function client(args)
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

    % Create a proxy to the weather station.
    weatherStation = WeatherStationPrx(communicator, 'weatherStation:tcp -h localhost -p 4061');

    % Create an AtmosphericConditions object with random values.
    temperature = randi([190, 230]) / 10; % Temperature in degrees Celsius (19.0 to 23.0).
    humidity = randi([450, 550]) / 10; % Humidity in percentage (45.0 to 55.0).

    reading = AtmosphericConditions(temperature, humidity);

    % Report this reading to the weather station.
    weatherStation.report('sensor-1', reading);

    fprintf('sensor v1: sent reading to weather station\n');
end
