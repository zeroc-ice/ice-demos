function client(args)
    %CLIENT An Ice client that connects to a Greeter server using IceDiscovery.

    % Copyright (c) ZeroC, Inc.

    arguments (Repeating)
        args (1, :) char
    end

    % Load the Ice library if it is not already loaded.
    if ~libisloaded('ice')
        loadlibrary('ice', @iceproto);
    end

    % Configure the communicator to load the IceDiscovery plug-in during initialization. This plug-in installs a default
    % locator on the communicator.
    properties = Ice.Properties(args);
    properties.setProperty('Ice.Plugin.IceDiscovery', '1');

    % Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
    communicator = Ice.Communicator(Properties = properties);

    % Destroy the communicator when the function exits.
    cleanup = onCleanup(@() communicator.destroy());

    % Create a proxy to the Greeter object hosted by the server(s). 'greeter' is a stringified proxy with no addressing
    % information, also known as a well-known proxy. It's resolved by the default locator installed by the IceDiscovery
    % plug-in.
    greeter = visitorcenter.GreeterPrx(communicator, 'greeter');

    % Send a request to the remote object and wait for the response.
    greeting = greeter.greet(char(java.lang.System.getProperty('user.name')));
    fprintf('%s\n', greeting);
end
