% **********************************************************************
% Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
% **********************************************************************

classdef Client
    methods(Static)
        function menu()
            fprintf([...
                'usage:\n', ...
                '\n', ...
                'toggle type of data to send:\n', ...
                '1: sequence of bytes (default)\n', ...
                '2: sequence of strings (''hello'')\n', ...
                '3: sequence of structs with a string (''hello'') and a double\n', ...
                '4: sequence of structs with two ints and a double\n', ...
                '\n', ...
                'select test to run:\n', ...
                't: Send sequence as twoway\n', ...
                'o: Send sequence as oneway\n', ...
                'r: Receive sequence\n', ...
                'e: Echo (send and receive) sequence\n', ...
                '\n', ...
                'other commands:\n', ...
                's: shutdown server\n', ...
                'x: exit\n', ...
                '?: help\n']);
        end
        function run(communicator)
            import Demo.*;

            throughput = ThroughputPrx.checkedCast(communicator.propertyToProxy('Throughput.Proxy'));
            if isempty(throughput)
                fprintf('invalid proxy\n');
                return;
            end

            throughputOneway = throughput.ice_oneway();

            byteSeq = zeros(1, ByteSeqSize.value, 'uint8');

            stringSeq = cell(1, StringSeqSize.value);
            for i = 1:StringSeqSize.value
                stringSeq{i} = 'hello';
            end

            structSeq(1, StringDoubleSeqSize.value) = StringDouble('hello', 3.14);

            fixedSeq(1, FixedSeqSize.value) = Fixed(0, 0, 0);

            %
            % To allow cross-language tests we may need to "warm up" the
            % server. The warm up is to ensure that any JIT compiler will
            % have converted any hotspots to native code. This ensures an
            % accurate throughput measurement.
            %
            if throughput.needsWarmup()
                emptyBytes = zeros(1, 1, 'uint8');
                emptyStrings = cell(1, 1);
                emptyStructs = cell(1, 1);
                emptyStructs{1} = StringDouble();
                emptyFixed = cell(1, 1);
                emptyFixed{1} = Fixed();

                throughput.startWarmup();

                fprintf('warming up the client/server...');
                for i = 1:10000
                    throughput.sendByteSeq(emptyBytes);
                    throughput.sendStringSeq(emptyStrings);
                    throughput.sendStructSeq(emptyStructs);
                    throughput.sendFixedSeq(emptyFixed);

                    throughput.recvByteSeq();
                    throughput.recvStringSeq();
                    throughput.recvStructSeq();
                    throughput.recvFixedSeq();

                    throughput.echoByteSeq(emptyBytes);
                    throughput.echoStringSeq(emptyStrings);
                    throughput.echoStructSeq(emptyStructs);
                    throughput.echoFixedSeq(emptyFixed);
                end
                throughput.endWarmup();

                fprintf(' ok\n');
            end

            Client.menu();

            currentType = '1';
            seqSize = ByteSeqSize.value;

            % Initial ping to setup the connection.
            throughput.ice_ping();

            while true
                line = input('==> ', 's');
                if isempty(line)
                    break;
                end

                %repetitions = 100;
                repetitions = 10;

                if line(1) == '1' || line(1) == '2' || line(1) == '3' || line(1) == '4'
                    currentType = line(1);

                    switch currentType
                        case '1'
                            fprintf('using byte sequences\n');
                            seqSize = ByteSeqSize.value;

                        case '2'
                            fprintf('using string sequences\n');
                            seqSize = StringSeqSize.value;

                        case '3'
                            fprintf('using variable-length struct sequences\n');
                            seqSize = StringDoubleSeqSize.value;

                        case '4'
                            fprintf('using fixed-length struct sequences\n');
                            seqSize = FixedSeqSize.value;
                    end
                elseif line(1) == 't' || line(1) == 'o' || line(1) == 'r' || line(1) == 'e'
                    if currentType == '1'
                        %repetitions = 1000; % Use more iterations for byte sequences as it's a lot faster
                        repetitions = 100;
                    end

                    c = line(1);

                    switch(c)
                        case 't'
                            fprintf('sending');

                        case 'o'
                            fprintf('sending');

                        case 'r'
                            fprintf('receiving');

                        case 'e'
                            fprintf('sending and receiving');
                    end

                    fprintf(' %d', repetitions);
                    switch currentType
                        case '1'
                            fprintf(' byte');

                        case '2'
                            fprintf(' string');

                        case '3'
                            fprintf(' variable-length struct');

                        case '4'
                            fprintf(' fixed-length struct');
                    end

                    fprintf(' sequences of size %d', seqSize);

                    if c == 'o'
                        fprintf(' as oneway');
                    end

                    fprintf('...\n');

                    tic();

                    for i = 1:repetitions
                        switch currentType
                            case '1'
                                switch c
                                    case 't'
                                        throughput.sendByteSeq(byteSeq);

                                    case 'o'
                                        throughputOneway.sendByteSeq(byteSeq);

                                    case 'r'
                                        throughput.recvByteSeq();

                                    case 'e'
                                        throughput.echoByteSeq(byteSeq);
                                end

                            case '2'
                                switch c
                                    case 't'
                                        throughput.sendStringSeq(stringSeq);

                                    case 'o'
                                        throughputOneway.sendStringSeq(stringSeq);

                                    case 'r'
                                        throughput.recvStringSeq();

                                    case 'e'
                                        throughput.echoStringSeq(stringSeq);
                                end

                            case '3'
                                switch c
                                    case 't'
                                        throughput.sendStructSeq(structSeq);

                                    case 'o'
                                        throughputOneway.sendStructSeq(structSeq);

                                    case 'r'
                                        throughput.recvStructSeq();

                                    case 'e'
                                        throughput.echoStructSeq(structSeq);
                                end

                            case '4'
                                switch c
                                    case 't'
                                        throughput.sendFixedSeq(fixedSeq);

                                    case 'o'
                                        throughputOneway.sendFixedSeq(fixedSeq);

                                    case 'r'
                                        throughput.recvFixedSeq();

                                    case 'e'
                                        throughput.echoFixedSeq(fixedSeq);
                                end
                        end
                    end

                    elaps = toc() * 1000.0;
                    fprintf('time for %d sequences: %f ms\n', repetitions, elaps);
                    fprintf('time per sequence: %f ms\n', elaps / repetitions);
                    wireSize = 0;
                    switch currentType
                        case '1'
                            wireSize = 1;

                        case '2'
                            wireSize = length(stringSeq{1});

                        case '3'
                            wireSize = length(structSeq(1).s);
                            wireSize = wireSize + 8; % Size of double on the wire.

                        case '4'
                            wireSize = 16; % Size of two ints and a double on the wire.
                    end
                    mbit = repetitions * seqSize * wireSize * 8.0 / elaps / 1000.0;
                    if c == 'e'
                        mbit = mbit * 2;
                    end
                    fprintf('throughput: %f Mbps\n', mbit);
                elseif line(1) == 's'
                    throughput.shutdown();
                elseif line(1) == 'x'
                    break;
                elseif line(1) == '?'
                    Client.menu();
                else
                    fprintf('unknown command `%s''\n', line);
                    Client.menu();
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
