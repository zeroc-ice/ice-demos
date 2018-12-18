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

        proxy = ContextPrx.checkedCast(communicator.propertyToProxy('Context.Proxy'));
        if isempty(proxy)
            fprintf('invalid proxy\n');
            return;
        end

        menu();

        while true
            line = input('==> ', 's');

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
                    menu();
                case ''
                otherwise
                    fprintf('unknown command `%s''\n', line);
                    menu();
            end
        end
    catch ex
        fprintf('%s\n', getReport(ex));
    end
    rmpath('generated');
end

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
