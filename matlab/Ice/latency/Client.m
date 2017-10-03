%{
**********************************************************************

Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.

This copy of Ice is licensed to you under the terms described in the
ICE_LICENSE file included in this distribution.

**********************************************************************
%}

classdef Client
    methods(Static)
        function r = run(communicator)
            import Demo.*;

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
        end
        function main()
            addpath('generated');
            if ~libisloaded('icematlab')
                loadlibrary('icematlab')
            end

            % Initializes a communicator and then destroys it when cleanup is collected
            communicator = Ice.initialize({'--Ice.Config=config.client'});
            cleanup = onCleanup(@() communicator.destroy());
            Client.run(communicator);
        end
    end
end
