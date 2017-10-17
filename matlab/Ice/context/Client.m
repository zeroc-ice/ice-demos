% **********************************************************************
% Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
% **********************************************************************

classdef Client
    methods(Static)
        function menu()
            fprintf(['usage:\n'...
                     '1: call with no request context\n'...
                     '2: call with explicit request context\n'...
                     '3: call with per-proxy request context\n'...
                     '4: call with implicit request context\n'...
                     's: shutdown server\n'...
                     'x: exit\n'...
                     '?: help\n']);
        end
        function run(communicator)
            import Demo.*;

            proxy = ContextPrx.checkedCast(communicator.propertyToProxy('Context.Proxy'));
            if isempty(proxy)
                fprintf('invalid proxy\n');
                return;
            end

            Client.menu();

            while true
                line = input('==> ', 's');
                if isempty(line)
                    break;
                end

                try
                    switch line
                        case '1'
                            proxy.call();
                        case '2'
                            ctx = containers.Map('KeyType', 'char', 'ValueType', 'char');
                            ctx('type') = 'Explicit';
                            proxy.ice_context(ctx).call();
                        case '3'
                            ctx = containers.Map('KeyType', 'char', 'ValueType', 'char');
                            ctx('type') = 'Per-Proxy';
                            proxy.ice_context(ctx).call();
                        case '4'
                            implicitContext = communicator.getImplicitContext();
                            ctx = containers.Map('KeyType', 'char', 'ValueType', 'char');
                            ctx('type') = 'Implicit';
                            implicitContext.setContext(ctx);
                            proxy.call();
                            implicitContext.setContext(containers.Map('KeyType', 'char', 'ValueType', 'char'));
                        case 's'
                            proxy.shutdown();
                        case 'x'
                            break;
                        case '?'
                            Client.menu();
                        otherwise
                            fprintf('unknown command `%s''\n', line);
                            Client.menu();
                    end
                catch ex
                    if isa(ex, 'Ice.Exception')
                        fprintf('%s\n', getReport(ex));
                    else
                        rethrow(ex);
                    end
                end
            end
        end
        function main()
            addpath('generated');
            if ~libisloaded('ice')
                loadlibrary('ice', @iceproto)
            end

            % Initializes a communicator and then destroys it when cleanup is collected
            communicator = Ice.initialize({'--Ice.Config=config.client'});
            cleanup = onCleanup(@() communicator.destroy());
            Client.run(communicator);
        end
    end
end
