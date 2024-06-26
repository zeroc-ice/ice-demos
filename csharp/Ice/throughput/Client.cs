// Copyright (c) ZeroC, Inc.

using Demo;

internal class Client
{
    public static int Main(string[] args)
    {
        int status = 0;

        try
        {
            // The new communicator is automatically destroyed (disposed) at the end of the
            // using statement
            using var communicator = Ice.Util.initialize(ref args, "config.client");
            // The communicator initialization removes all Ice-related arguments from args
            if (args.Length > 0)
            {
                Console.Error.WriteLine("too many arguments");
                status = 1;
            }
            else
            {
                status = Run(communicator);
            }
        }
        catch (Exception ex)
        {
            Console.Error.WriteLine(ex);
            status = 1;
        }

        return status;
    }

    private static int Run(Ice.Communicator communicator)
    {
        var throughput = ThroughputPrxHelper.checkedCast(communicator.propertyToProxy("Throughput.Proxy"));
        if (throughput == null)
        {
            Console.Error.WriteLine("invalid proxy");
            return 1;
        }
        var throughputOneway = ThroughputPrxHelper.uncheckedCast(throughput.ice_oneway());

        var byteSeq = new byte[ByteSeqSize.value];

        var stringSeq = new string[StringSeqSize.value];
        for (int i = 0; i < StringSeqSize.value; ++i)
        {
            stringSeq[i] = "hello";
        }

        var structSeq = new StringDouble[StringDoubleSeqSize.value];
        for (int i = 0; i < StringDoubleSeqSize.value; ++i)
        {
            structSeq[i] = new StringDouble();
            structSeq[i].s = "hello";
            structSeq[i].d = 3.14;
        }

        var fixedSeq = new Fixed[FixedSeqSize.value];
        for (int i = 0; i < FixedSeqSize.value; ++i)
        {
            fixedSeq[i].i = 0;
            fixedSeq[i].j = 0;
            fixedSeq[i].d = 0;
        }

        // A method needs to be invoked thousands of times before the
        // JIT compiler will convert it to native code. To ensure an
        // accurate throughput measurement, we need to "warm up" the
        // JIT compiler.
        {
            var warmupBytes = new byte[1];
            var warmupStrings = new string[1];
            var warmupStructs = new StringDouble[1];
            warmupStructs[0] = new StringDouble();
            var warmupFixed = new Fixed[1];

            throughput.startWarmup();

            Console.Error.Write("warming up the client/server...");
            Console.Error.Flush();
            for (int i = 0; i < 10000; i++)
            {
                throughput.sendByteSeq(warmupBytes);
                throughput.sendStringSeq(warmupStrings);
                throughput.sendStructSeq(warmupStructs);
                throughput.sendFixedSeq(warmupFixed);

                throughput.recvByteSeq();
                throughput.recvStringSeq();
                throughput.recvStructSeq();
                throughput.recvFixedSeq();

                throughput.echoByteSeq(warmupBytes);
                throughput.echoStringSeq(warmupStrings);
                throughput.echoStructSeq(warmupStructs);
                throughput.echoFixedSeq(warmupFixed);
            }
            throughput.endWarmup();

            Console.Error.WriteLine("ok");
        }

        Menu();

        // By default use byte sequence.
        char currentType = '1';
        int seqSize = ByteSeqSize.value;

        string line = null;
        do
        {
            try
            {
                Console.Write("==> ");
                Console.Out.Flush();
                line = Console.In.ReadLine();
                if (line == null)
                {
                    break;
                }

                long tmsec = DateTime.Now.Ticks / 10000;
                int repetitions = 100;

                if (line.Equals("1") || line.Equals("2") || line.Equals("3") || line.Equals("4"))
                {
                    currentType = line[0];
                    switch (currentType)
                    {
                        case '1':
                        {
                            Console.WriteLine("using byte sequences");
                            seqSize = ByteSeqSize.value;
                            break;
                        }
                        case '2':
                        {
                            Console.WriteLine("using string sequences");
                            seqSize = StringSeqSize.value;
                            break;
                        }
                        case '3':
                        {
                            Console.WriteLine("using variable-length struct sequences");
                            seqSize = StringDoubleSeqSize.value;
                            break;
                        }
                        case '4':
                        {
                            Console.WriteLine("using fixed-length struct sequences");
                            seqSize = FixedSeqSize.value;
                            break;
                        }
                    }
                }
                else if (line.Equals("t") || line.Equals("o") || line.Equals("r") || line.Equals("e"))
                {
                    if (currentType == '1')
                    {
                        repetitions = 1000; // Use more iterations for byte sequences as it's a lot faster
                    }

                    char c = line[0];
                    switch (c)
                    {
                        case 't':
                        {
                            Console.Write("sending");
                            break;
                        }
                        case 'o':
                        {
                            Console.Write("sending");
                            break;
                        }
                        case 'r':
                        {
                            Console.Write("receiving");
                            break;
                        }
                        case 'e':
                        {
                            Console.Write("sending and receiving");
                            break;
                        }
                    }

                    Console.Write(" " + repetitions);
                    switch (currentType)
                    {
                        case '1':
                        {
                            Console.Write(" byte");
                            break;
                        }
                        case '2':
                        {
                            Console.Write(" string");
                            break;
                        }
                        case '3':
                        {
                            Console.Write(" variable-length struct");
                            break;
                        }
                        case '4':
                        {
                            Console.Write(" fixed-length struct");
                            break;
                        }
                    }
                    Console.Write(" sequences of size " + seqSize);

                    if (c == 'o')
                    {
                        Console.Write(" as oneway");
                    }

                    Console.WriteLine("...");

                    for (int i = 0; i < repetitions; ++i)
                    {
                        switch (currentType)
                        {
                            case '1':
                            {
                                switch (c)
                                {
                                    case 't':
                                    {
                                        throughput.sendByteSeq(byteSeq);
                                        break;
                                    }

                                    case 'o':
                                    {
                                        throughputOneway.sendByteSeq(byteSeq);
                                        break;
                                    }

                                    case 'r':
                                    {
                                        throughput.recvByteSeq();
                                        break;
                                    }

                                    case 'e':
                                    {
                                        throughput.echoByteSeq(byteSeq);
                                        break;
                                    }
                                }
                                break;
                            }

                            case '2':
                            {
                                switch (c)
                                {
                                    case 't':
                                    {
                                        throughput.sendStringSeq(stringSeq);
                                        break;
                                    }

                                    case 'o':
                                    {
                                        throughputOneway.sendStringSeq(stringSeq);
                                        break;
                                    }

                                    case 'r':
                                    {
                                        throughput.recvStringSeq();
                                        break;
                                    }

                                    case 'e':
                                    {
                                        throughput.echoStringSeq(stringSeq);
                                        break;
                                    }
                                }
                                break;
                            }

                            case '3':
                            {
                                switch (c)
                                {
                                    case 't':
                                    {
                                        throughput.sendStructSeq(structSeq);
                                        break;
                                    }

                                    case 'o':
                                    {
                                        throughputOneway.sendStructSeq(structSeq);
                                        break;
                                    }

                                    case 'r':
                                    {
                                        throughput.recvStructSeq();
                                        break;
                                    }

                                    case 'e':
                                    {
                                        throughput.echoStructSeq(structSeq);
                                        break;
                                    }
                                }
                                break;
                            }

                            case '4':
                            {
                                switch (c)
                                {
                                    case 't':
                                    {
                                        throughput.sendFixedSeq(fixedSeq);
                                        break;
                                    }

                                    case 'o':
                                    {
                                        throughputOneway.sendFixedSeq(fixedSeq);
                                        break;
                                    }

                                    case 'r':
                                    {
                                        throughput.recvFixedSeq();
                                        break;
                                    }

                                    case 'e':
                                    {
                                        throughput.echoFixedSeq(fixedSeq);
                                        break;
                                    }
                                }
                                break;
                            }
                        }
                    }

                    double dmsec = (DateTime.Now.Ticks / 10000) - tmsec;
                    Console.WriteLine("time for " + repetitions + " sequences: " + dmsec.ToString("F") + "ms");
                    Console.WriteLine("time per sequence: " + ((double)(dmsec / repetitions)).ToString("F") + "ms");
                    int wireSize = 0;
                    switch (currentType)
                    {
                        case '1':
                        {
                            wireSize = 1;
                            break;
                        }
                        case '2':
                        {
                            wireSize = stringSeq[0].Length;
                            break;
                        }
                        case '3':
                        {
                            wireSize = structSeq[0].s.Length;
                            wireSize += 8; // Size of double on the wire.
                            break;
                        }
                        case '4':
                        {
                            wireSize = 16; // Sizes of two ints and a double on the wire.
                            break;
                        }
                    }
                    double mbit = repetitions * seqSize * wireSize * 8.0 / dmsec / 1000.0;
                    if (c == 'e')
                    {
                        mbit *= 2;
                    }
                    Console.WriteLine("throughput: " + mbit.ToString("#.##") + "Mbps");
                }
                else if (line.Equals("s"))
                {
                    throughput.shutdown();
                }
                else if (line.Equals("x"))
                {
                    // Nothing to do
                }
                else if (line.Equals("?"))
                {
                    Menu();
                }
                else
                {
                    Console.WriteLine("unknown command `" + line + "'");
                    Menu();
                }
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine(ex);
            }
        }
        while (!line.Equals("x"));

        return 0;
    }

    private static void Menu()
    {
        Console.WriteLine("usage:\n"
                          + "\n"
                          + "toggle type of data to send:\n"
                          + "1: sequence of bytes (default)\n"
                          + "2: sequence of strings (\"hello\")\n"
                          + "3: sequence of structs with a string (\"hello\") and a double\n"
                          + "4: sequence of structs with two ints and a double\n"
                          + "\n"
                          + "select test to run:\n"
                          + "t: Send sequence as twoway\n"
                          + "o: Send sequence as oneway\n"
                          + "r: Receive sequence\n"
                          + "e: Echo (send and receive) sequence\n"
                          + "\n"
                          + "other commands\n"
                          + "s: shutdown server\n"
                          + "x: exit\n"
                          + "?: help\n");
    }
}
