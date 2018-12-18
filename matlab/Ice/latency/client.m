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
        communicator = Ice.initialize({'--Ice.Config=config.client'});
        cleanup = onCleanup(@() communicator.destroy());

        ping = PingPrx.checkedCast(communicator.propertyToProxy('Ping.Proxy'));
        if isempty(ping)
            fprintf('invalid proxy\n');
            return;
        end

        repetitions = 50000;
        fprintf('pinging server %d times (this may take a while)\n', repetitions);

        tic();

        for i = 1:repetitions
            ping.ice_ping();
        end

        total = toc() * 1000.0;

        fprintf('time for %d pings: %f ms\n', repetitions, total);
        fprintf('time per ping: %f ms\n', total / repetitions);
    catch ex
        fprintf('%s\n', getReport(ex));
    end
    rmpath('generated');
end
