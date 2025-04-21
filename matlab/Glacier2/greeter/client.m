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

    % Create a proxy to the Glacier2 router. The addressing information (transport, host and port number) is derived
    % from the value of Glacier2.Client.Endpoints in the Glacier2 router configuration file.
    router = Glacier2.RouterPrx(communicator, 'Glacier2/router:tcp -h localhost -p 4063');

    % Retrieve my username.
    username = char(java.lang.System.getProperty('user.name'));

    % Create a session with the Glacier2 router. In this demo, the Glacier2 router is configured to accept any
    % username/password combination. This call establishes a network connection to the Glacier2 router; the lifetime of
    % the session is the same as the lifetime of the connection.
    session = router.createSession(username, 'password');

    % The proxy returned by createSession is null because we did not configure a SessionManager on the Glacier2 router.
    assert(isempty(session));

    % Create a proxy to the Greeter object in the server behind the Glacier2 router. Typically, the client cannot
    % connect directly to this server because it's on an unreachable network.
    greeter = VisitorCenter.GreeterPrx(communicator, 'greeter:tcp -h localhost -p 4061');

    % Configure the proxy to route requests using the Glacier2 router.
    greeter = greeter.ice_router(router);

    % Send a request to the remote object and get the response.
    greeting = greeter.greet(userName);
    fprintf('%s\n', greeting);

    % Send a second request to observe the effect in the Glacier2 router log.
    greeting = greeter.greet('alice');
    fprintf('%s\n', greeting);
end
