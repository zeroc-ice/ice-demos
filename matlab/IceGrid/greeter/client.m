% Copyright (c) ZeroC, Inc.

function client(args)
    % The Slice module VisitorCenter maps to a MATLAB namespace with the same name.
    import VisitorCenter.*

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

    % Set the default locator of the new communicator. It's the address of the Locator hosted by our IceGrid registry.
    % You can also set this proxy with the Ice.Default.Locator property.
    communicator.setDefaultLocator(Ice.LocatorPrx(communicator, 'IceGrid/Locator:tcp -h localhost -p 4061'));

    % Create a proxy to the Greeter object hosted by the server. 'greeter' is a stringified proxy with no addressing
    % information, also known as a well-known proxy. It's specified by the <object> element in the IceGrid XML file.
    % The IceGrid registry resolves this well-known proxy and returns the actual address (endpoint) of the server to
    % this client.
    greeter = VisitorCenter.GreeterPrx(communicator, 'greeter');

    % Send a request to the remote object and get the response.
    greeting = greeter.greet(char(java.lang.System.getProperty('user.name')));
    fprintf('%s\n', greeting);

    % Send another request to the remote object. With the default configuration we use for this client, this request
    % reuses the connection and reaches the same server, even when we have multiple replicated servers.
    greeting = greeter.greet('alice');
    fprintf('%s\n', greeting);
end
