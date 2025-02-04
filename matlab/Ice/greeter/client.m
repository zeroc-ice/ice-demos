% Copyright (c) ZeroC, Inc.

function client(args)
    % We put the code generated by the Slice compiler in the generated subdirectory.
    addpath('generated');

    % Load the Ice library if it is not already loaded.
    if ~libisloaded('ice')
        loadlibrary('ice', @iceproto);
    end

    % The Slice module VisitorCenter maps to a MATLAB package with the same name.
    import VisitorCenter.*

    if nargin == 0
        args = {};
    end

    % Create an Ice communicator to initialize the Ice runtime, and destroy this communicator at the end of this
    % function.
    communicator = Ice.initialize(args);
    cleanup = onCleanup(@() communicator.destroy());

    % GreeterPrx is a class generated by the Slice compiler. We create a proxy from a communicator and a
    % "stringified proxy" with the address of the target object.
    % If you run the server on a different computer, replace localhost in the string below with the server's hostname
    % or IP address.
    greeter = VisitorCenter.GreeterPrx(communicator, 'greeter:tcp -h localhost -p 4061');

    % Send a request to the remote object and wait synchronously for the response.
    greeting = greeter.greet(char(java.lang.System.getProperty('user.name')));

    fprintf('%s\n', greeting);

    % Send another request to the remote object, this time with greetAsync. greetAsync returns a future immediately.
    futureGreeting = greeter.greetAsync('alice');

    % Wait for the response.
    greeting = futureGreeting.fetchOutputs();

    fprintf('%s\n', greeting);

    rmpath('generated');
end
