% Copyright (c) ZeroC, Inc.

function client(args)
    % The Slice module VisitorCenter maps to a MATLAB package with the same name.
    import VisitorCenter.*

    if nargin == 0
        args = {};
    end

    % Load the Ice library if it is not already loaded.
    if ~libisloaded('ice')
        loadlibrary('ice', @iceproto);
    end

    % Configure the communicator to load the IceDiscovery plugin during initialization. This plugin installs a default
    % locator on the communicator.
    initData = Ice.InitializationData();
    initData.properties_ = Ice.createProperties(args);
    initData.properties_.setProperty('Ice.Plugin.IceDiscovery', '1');

    % Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
    communicator = Ice.initialize(initData);

    % Destroy the communicator when the function exits.
    cleanup = onCleanup(@() communicator.destroy());

    % Create a proxy to the Greeter object hosted by the server(s). 'greeter' is a stringified proxy with no addressing
    % information, also known as a well-known proxy. It's resolved by the default locator installed by the IceDiscovery
    % plugin.
    greeter = GreeterPrx(communicator, 'greeter');

    % Send a request to the remote object and wait for the response.
    greeting = greeter.greet(char(java.lang.System.getProperty('user.name')));
    fprintf('%s\n', greeting);
end
