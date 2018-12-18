% **********************************************************************
% Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
% **********************************************************************

classdef UI < handle

    methods

        function obj = UI(obj)
            try
                f = figure('Visible', 'off',...
                           'Position', [100 100 400 280]);

                uicontrol('Style', 'text',...
                          'String', 'Host:',...
                          'Position', [20, 235, 50, 20]);

                obj.host = uicontrol('Style', 'edit',...
                                     'String', '127.0.0.1',...
                                     'HorizontalAlignment', 'left',...
                                     'Position', [65, 240, 300, 20],...
                                     'Callback', @(~, ~) obj.updateProxy());

                uicontrol('Style', 'text',...
                          'String', 'Mode:',...
                          'Position', [20 205 50 20]);

                obj.mode = uicontrol('Style', 'popup',...
                                     'String', {'Twoway',...
                                                'Twoway Secure',...
                                                'Oneway',...
                                                'Oneway Batch',...
                                                'Oneway Secure',...
                                                'Oneway Secure Batch',...
                                                'Datagram',...
                                                'Datagram Batch'},...
                                     'Position', [65 210 300 20],...
                                     'Callback', @(~, ~) obj.updateProxy());

                uicontrol('Style', 'text',...
                          'String', 'Timeout:',...
                          'Position', [15 175 50 20]);

                obj.timeout = uicontrol('Style', 'slider',...
                                        'Min', 0,...
                                        'Max', 5000,...
                                        'Value', 0,...
                                        'Position', [65 180 300 20],...
                                        'Callback', @(~, ~) obj.updateProxy());

                uicontrol('Style', 'text',...
                          'String', 'Delay:',...
                          'Position', [20 145 50 20]);

                obj.delay = uicontrol('Style', 'slider',...
                                      'Min', 0,...
                                      'Max', 5000,...
                                      'Value', 0,...
                                      'Position', [65 150 300 20]);

                uicontrol('Style', 'pushbutton',...
                          'String', 'Hello',...
                          'Position', [20 120 60 20],...
                          'Callback', @(~, ~) obj.sayHello());

                uicontrol('Style', 'pushbutton',...
                          'String', 'Shutdown',...
                          'Position', [90 120 60 20],...
                          'Callback', @(~, ~) obj.shutdown());

                obj.flushBtn = uicontrol('Style', 'pushbutton',...
                                         'String', 'Flush',...
                                         'Position', [160 120 60 20],...
                                         'Enable', 'Inactive',...
                                         'Callback', @(~, ~) obj.flush());

                obj.status = uicontrol('Style', 'text',...
                                       'String', 'Ready',...
                                       'HorizontalAlignment', 'left',...
                                       'Position', [20, 5, 350, 110]);

                movegui('center');
                f.Visible = 'on';
                obj.communicator = Ice.initialize({'--Ice.Config=config.client'});
                obj.updateProxy();
            catch ex
                obj.status.String = class(ex);
                fprintf('%s\n', getReport(ex));
            end
        end

        function updateProxy(obj)
            hostname = obj.host.String;
            if isempty(hostname)
                obj.status.String = 'No hostname';
                return;
            end

            proxy = obj.communicator.stringToProxy(sprintf('hello:tcp -h %s -p 10000:udp -h %s -p 10000:ssl -p 10001 -h %s',...
                hostname, hostname, hostname));

            m = obj.mode.String{obj.mode.Value};
            switch m
                case 'Twoway'
                    proxy = proxy.ice_twoway();
                case 'Twoway Secure'
                    proxy = proxy.ice_twoway().ice_secure(1);
                case 'Oneway'
                    proxy = proxy.ice_oneway();
                case 'Oneway Batch'
                    proxy = proxy.ice_batchOneway();
                case 'Oneway Secure Batch'
                    proxy = proxy.ice_batchOneway().ice_secure(1);
                case 'Datagram'
                    proxy = proxy.ice_datagram();
                case 'Datagram Batch'
                    proxy = proxy.ice_batchDatagram();
            end

            if obj.timeout.Value > 0
                proxy = proxy.ice_invocationTimeout(obj.timeout.Value);
            end

            %
            % The batch requests associated to the proxy are lost when we
            % update the proxy.
            %
            obj.flushBtn.Enable = 'off';
            obj.status.String = 'Ready';

            obj.helloPrx = Demo.HelloPrx.uncheckedCast(proxy);
        end

        function shutdown(obj)
            m = obj.mode.String{obj.mode.Value};
            if strfind(m, 'Batch')
                try
                    obj.flushBtn.Enable = 'on';
                    obj.helloPrx.shutdown();
                    obj.status.String = 'Queued shutdown request';
                catch ex
                    obj.status.String = class(ex);
                    fprintf('%s\n', getReport(ex));
                end
            else
                obj.status.String = 'Sending request';
                obj.waitForComplete(obj.helloPrx.shutdownAsync());
            end
        end

        function flush(obj)
            obj.flushBtn.Enable = 'off';
            obj.status.String = 'Flushed batch requests';
            obj.waitForComplete(obj.helloPrx.ice_flushBatchRequestsAsync());
        end

        function sayHello(obj)
            m = obj.mode.String{obj.mode.Value};
            if strfind(m, 'Batch')
                try
                    obj.flushBtn.Enable = 'on';
                    obj.helloPrx.sayHello(obj.delay.Value);
                    obj.status.String = 'Queued sayHello request';
                catch ex
                    obj.status.String = class(ex);
                    fprintf('%s\n', getReport(ex));
                end
            else
                obj.status.String = 'Sending request';
                obj.waitForComplete(obj.helloPrx.sayHelloAsync(obj.delay.Value));
            end
        end

        function delete(obj)
            obj.communicator.destroy();
            rmpath('generated');
        end

        function waitForComplete(obj, f)
            t = timer;
            t.ExecutionMode = 'fixedRate';
            t.Period = 0.1;
            function completed(t, ~)
                if strcmp(f.State, 'finished')
                    stop(t);
                    delete(t);
                    try
                        f.fetchOutputs();
                        obj.status.String = 'Ready';
                    catch ex
                        obj.status.String = class(ex);
                        if ~isa(ex, 'Ice.LocalException')
                            fprintf('%s\n', getReport(ex));
                        end
                    end
                end
            end
            t.TimerFcn = @completed;
            start(t);
        end
    end

    properties
        communicator
        helloPrx
        host
        mode
        timeout
        delay
        status
        flushBtn
    end
end
