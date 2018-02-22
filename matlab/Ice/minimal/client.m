% **********************************************************************
% Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
% **********************************************************************

function client(args)
    try
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
    catch ex
        fprintf('%s\n', getReport(ex));
    end
end
