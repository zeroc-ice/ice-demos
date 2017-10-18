% **********************************************************************
% Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
% **********************************************************************

classdef Client
    methods(Static)

        function menu()
            fprintf(['usage:\n'...
                     '1: set properties (batch 1)\n'...
                     '2: set properties (batch 2)\n'...
                     'c: show current properties\n'...
                     's: shutdown server\n'...
                     'x: exit\n'...
                     '?: help\n']);
        end

        function run(communicator)
            import Demo.*;

            props = PropsPrx.checkedCast(communicator.propertyToProxy('Props.Proxy'));
            if isempty(props)
                fprintf('invalid proxy\n');
                return;
            end

            admin = Ice.PropertiesAdminPrx.checkedCast(communicator.propertyToProxy('Admin.Proxy'));

            propertyKeys = {'Demo.Prop1', 'Demo.Prop2', 'Demo.Prop3'};

            batch1 = containers.Map('KeyType', 'char', 'ValueType', 'char');
            batch1('Demo.Prop1') = '1';
            batch1('Demo.Prop2') = '2';
            batch1('Demo.Prop3') = '3';

            batch2 = containers.Map('KeyType', 'char', 'ValueType', 'char');
            batch2('Demo.Prop1') = '10';
            batch2('Demo.Prop2') = char;
            batch2('Demo.Prop3') = '30';

            Client.show(admin);
            Client.menu();

            while true
                line = input('==> ', 's');

                try
                    switch line
                        case {'1', '2'}
                            if strcmp(line, '1')
                                dict = batch1;
                            else
                                dict = batch2;
                            end

                            fprintf('Sending:\n');
                            for i = 1:length(propertyKeys)
                                key = propertyKeys{i};
                                value = dict(key);
                                if ~isempty(value)
                                    fprintf('  %s=%s\n', key, value);
                                end
                            end
                            fprintf('\n');

                            admin.setProperties(dict);

                            fprintf('Changes:\n');
                            changes = props.getChanges();
                            if isempty(changes)
                                fprintf('  None\n');
                            else
                                for i = 1:length(propertyKeys)
                                    key = propertyKeys{i};
                                    value = dict(key);
                                    if isempty(value)
                                        fprintf('  %s was removed\n', key);
                                    else
                                        fprintf('  %s is now %s\n', key, value);
                                    end
                                end
                            end
                        case 'c'
                            Client.show(admin);
                        case 's'
                            props.shutdown();
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
                         fprintf('%s\n', ex.ice_id());
                     else
                         fprintf('%s\n', class(ex));
                         fprintf('%s\n', getReport(ex));
                     end
                end
            end
        end

        function show(admin)
            props = admin.getPropertiesForPrefix('Demo');
            fprintf('Server''s current settings:\n');
            cellfun(@(key) fprintf('  %s=%s\n', key, props(key)), keys(props));
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
