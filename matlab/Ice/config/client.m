function client(args)
    %CLIENT An Ice client that demonstrates Ice properties and configuration.

    % Copyright (c) ZeroC, Inc.

    arguments (Repeating)
        args (1, :) char
    end

    % Load the Ice library if it is not already loaded.
    if ~libisloaded('ice')
        loadlibrary('ice', @iceproto);
    end

    % Load the contents of the config.client file into a Properties object.
    configFileProperties = Ice.Properties();
    configFileProperties.load('config.client');

    % Create a Properties object from the command line arguments and the config file properties; Ice.* properties and
    % other reserved properties set in args augment or override the config file properties.
    properties = Ice.Properties(args, configFileProperties);

    % Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
    % The communicator gets its properties from the properties object.
    communicator = Ice.Communicator(Properties = properties);

    % Destroy the communicator when the function exits.
    cleanup = onCleanup(@() communicator.destroy());

    % We create a Greeter proxy using the value of the Greeter.Proxy property in config.client.
    % It's null if the property is not set.
    greeter = visitorcenter.GreeterPrx.uncheckedCast(communicator.propertyToProxy('Greeter.Proxy'));

    % Send a request to the remote object and get the response.
    greeting = greeter.greet(char(java.lang.System.getProperty('user.name')));
    fprintf('%s\n', greeting);
end
