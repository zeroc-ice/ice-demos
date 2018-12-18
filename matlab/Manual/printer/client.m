% **********************************************************************
% Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
% **********************************************************************

function client()
    addpath('generated');
    if ~libisloaded('ice')
        loadlibrary('ice', @iceproto);
    end

    import Demo.*;

    try
        % Initializes a communicator and then destroys it when cleanup is collected
        communicator = Ice.initialize();
        cleanup = onCleanup(@() communicator.destroy());

        base = communicator.stringToProxy('SimplePrinter:default -h localhost -p 10000');
        printer = Demo.PrinterPrx.checkedCast(base);
        if isempty(printer)
            throw(MException('Client:RuntimeError', 'Invalid proxy'));
        end

        printer.printString('Hello World!');
    catch ex
        fprintf('%s\n', getReport(ex));
    end
    rmpath('generated');
end
