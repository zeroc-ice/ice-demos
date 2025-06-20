% Copyright (c) ZeroC, Inc.

function client(args)
    arguments (Repeating)
        args (1, :) char
    end

    % Load the Ice library if it is not already loaded.
    if ~libisloaded('ice')
        loadlibrary('ice', @iceproto);
    end

    % Create Ice properties from the contents of the config.client file in the current working directory.
    properties = Ice.Properties();
    properties.load('config.client');

    % Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
    % The communicator gets its properties from the Properties arguments; Ice.* properties set in args override these
    % properties.
    communicator = Ice.Communicator(args, Properties = properties);

    % Display the communicator's properties.
    fprintf('Communicator properties:\n');
    disp(communicator.getProperties());

    % Destroy the communicator when the function exits.
    cleanup = onCleanup(@() communicator.destroy());

    % We create a Greeter proxy using the value of the Greeter.Proxy property in config.client.
    % It's null if the property is not set.
    greeter = visitorcenter.GreeterPrx.uncheckedCast(communicator.propertyToProxy('Greeter.Proxy'));

    % Send a request to the remote object and get the response.
    greeting = greeter.greet(char(java.lang.System.getProperty('user.name')));
    fprintf('%s\n', greeting);
end
