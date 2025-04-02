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

    % Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections. The
    % communicator gets its configuration properties from file config.client in the client's current working directory.
    % The communicator initialization also parses args to find and set additional properties.
    communicator = Ice.initialize(args, 'config.client');

    % Destroy the communicator when the function exits.
    cleanup = onCleanup(@() communicator.destroy());

    % We create a Greeter proxy using the value of the Greeter.Proxy property in config.client.
    % It's null if the property is not set.
    greeter = GreeterPrx.uncheckedCast(communicator.propertyToProxy('Greeter.Proxy'));

    % Send a request to the remote object and get the response.
    greeting = greeter.greet(char(java.lang.System.getProperty('user.name')));
    fprintf('%s\n', greeting);
end
