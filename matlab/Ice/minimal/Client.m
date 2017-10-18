% **********************************************************************
% Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
% **********************************************************************

function Client(args)
    addpath('generated');
    if ~libisloaded('ice')
        loadlibrary('ice', @iceproto);
    end
    import Demo.*

    if nargin == 0
        args = {};
    end

    communicator = Ice.initialize(args);
    cleanup = onCleanup(@() communicator.destroy());
    hello = Demo.HelloPrx.checkedCast(communicator.stringToProxy('hello:default -h localhost -p 10000'));
    hello.sayHello();
end
