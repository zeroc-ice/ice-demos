% **********************************************************************
% Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
% **********************************************************************

function client()
    addpath('generated');
    if ~libisloaded('ice')
        loadlibrary('ice', @iceproto);
    end

    import Demo.*;

    % Initializes a communicator and then destroys it when cleanup is collected
    communicator = Ice.initialize({'--Ice.Config=config.client'});
    cleanup = onCleanup(@() communicator.destroy());

    twoway = HelloPrx.checkedCast(communicator.stringToProxy('hello'));
    if isempty(twoway)
        fprintf('invalid proxy\n');
        return;
    end

    oneway = twoway.ice_oneway();
    batchOneway = twoway.ice_batchOneway();
    datagram = twoway.ice_datagram();
    batchDatagram = twoway.ice_batchDatagram();

    secure = 0;
    timeout = -1;
    delay = 0;

    menu();

    while true
        try
            line = input('==> ', 's');
            switch line
                case 't'
                    twoway.sayHello(delay);
                case 'o'
                    oneway.sayHello(delay);
                case 'O'
                    batchOneway.sayHello(delay);
                case 'd'
                    if secure
                        fprintf('secure datagrams are not supported\n');
                    else
                        datagram.sayHello(delay);
                    end
                case 'D'
                    if secure
                        fprintf('secure datagrams are not supported\n');
                    else
                        batchDatagram.sayHello(delay);
                    end
                case 'f'
                    batchOneway.ice_flushBatchRequests();
                    if ~secure
                        batchDatagram.ice_flushBatchRequests();
                    end
                case 'T'
                    if timeout == -1
                        timeout = 2000;
                    else
                        timeout = -1;
                    end

                    twoway = twoway.ice_invocationTimeout(timeout);
                    oneway = oneway.ice_invocationTimeout(timeout);
                    batchOneway = batchOneway.ice_invocationTimeout(timeout);

                    if timeout == -1
                        fprintf('timeout is now switched off\n');
                    else
                        fprintf('timeout is now set to 2000ms\n');
                    end
                case 'P'
                    if delay == 0
                        delay = 2500;
                        fprintf('server delay is now set to 2500ms\n');
                    else
                        delay = 0;
                        fprintf('server delay is now deactivated\n');
                    end
                case 'S'
                    secure = not(secure);

                    twoway = twoway.ice_secure(secure);
                    oneway = oneway.ice_secure(secure);
                    batchOneway = batchOneway.ice_secure(secure);
                    datagram = datagram.ice_secure(secure);
                    batchDatagram = batchDatagram.ice_secure(secure);

                    if secure
                        fprintf('secure mode is now on\n');
                    else
                        fprintf('secure mode is now off\n');
                    end
                case 's'
                    twoway.shutdown();
                case 'x'
                    break;
                case '?'
                    menu();
                case ''
                otherwise
                    fprintf('unknown command `%s''\n', line);
                    menu();
            end
        catch ex
            fprintf('%s\n', getReport(ex));
        end
    end
    rmpath('generated');
end

function menu()
    fprintf(['usage:\n'...
             't: send greeting as twoway\n'...
             'o: send greeting as oneway\n'...
             'O: send greeting as batch oneway\n'...
             'd: send greeting as datagram\n'...
             'D: send greeting as batch datagram\n'...
             'f: flush all batch requests\n'...
             'T: set a timeout\n'...
             'P: set a server delay\n'...
             'S: switch secure mode on/off\n'...
             's: shutdown server\n'...
             'x: exit\n'...
             '?: help\n']);
end
