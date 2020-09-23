// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using System;
using System.Configuration;
using System.Diagnostics;
using ZeroC.Ice;

int status = 0;
try
{
    // The new communicator is automatically destroyed (disposed) at the end of the using statement
    var communicator = new Communicator(ref args, ConfigurationManager.AppSettings);

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

static int Run(Communicator communicator)
{
    IThroughputPrx throughput = communicator.GetPropertyAsProxy("Throughput.Proxy", IThroughputPrx.Factory) ??
        throw new ArgumentException("invalid proxy");

    byte[] byteSeq = new byte[Constants.ByteSeqSize];

    string[] stringSeq = new string[Constants.StringSeqSize];
    for (int i = 0; i < Constants.StringSeqSize; ++i)
    {
        stringSeq[i] = "hello";
    }

    var structSeq = new StringDouble[Constants.StringDoubleSeqSize];
    for (int i = 0; i < Constants.StringDoubleSeqSize; ++i)
    {
        structSeq[i] = new StringDouble("hello", 3.14);
    }

    var fixedSeq = new Fixed[Constants.FixedSeqSize];
    for (int i = 0; i < Constants.FixedSeqSize; ++i)
    {
        fixedSeq[i].I = 0;
        fixedSeq[i].J = 0;
        fixedSeq[i].D = 0;
    }

    //
    // A method needs to be invoked thousands of times before the
    // JIT compiler will convert it to native code. To ensure an
    // accurate throughput measurement, we need to "warm up" the
    // JIT compiler.
    //
    {
        byte[] warmupBytes = new byte[1];
        string[] warmupStrings = new string[] { "hello" };
        var warmupStructs = new StringDouble[] { new StringDouble("hello", 3.14) };
        var warmupFixed = new Fixed[1];

        throughput.StartWarmup();

        Console.Error.Write("warming up the client/server...");
        Console.Error.Flush();
        for (int i = 0; i < 10000; i++)
        {
            throughput.SendByteSeq(warmupBytes);
            throughput.SendStringSeq(warmupStrings);
            throughput.SendStructSeq(warmupStructs);
            throughput.SendFixedSeq(warmupFixed);

            throughput.RecvByteSeq();
            throughput.RecvStringSeq();
            throughput.RecvStructSeq();
            throughput.RecvFixedSeq();

            throughput.EchoByteSeq(warmupBytes);
            throughput.EchoStringSeq(warmupStrings);
            throughput.EchoStructSeq(warmupStructs);
            throughput.EchoFixedSeq(warmupFixed);
        }
        throughput.EndWarmup();

        Console.Error.WriteLine("ok");
    }

    Menu();

    //
    // By default use byte sequence.
    //
    char currentType = '1';
    int seqSize = Constants.ByteSeqSize;
    var watch = new Stopwatch();
    string? line = null;
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

            double repetitions = 100;

            if (line == "1" || line == "2" || line == "3" || line == "4")
            {
                if (line == "1")
                {
                    repetitions = 1000;
                }
                currentType = line[0];
                (int size, string message) = currentType switch
                {
                    '1' => (Constants.ByteSeqSize, "using byte sequences"),
                    '2' => (Constants.StringSeqSize, "using string sequences"),
                    '3' => (Constants.StringDoubleSeqSize, "using variable-length struct sequences"),
                    _ => (Constants.FixedSeqSize, "using fixed-length struct sequences")
                };
                Console.WriteLine(message);
                seqSize = size;
            }
            else if (line == "t" || line == "o" || line == "r" || line == "e")
            {
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
                watch.Restart();
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
                                    throughput.SendByteSeq(byteSeq);
                                    break;
                                }
                                case 'o':
                                {
                                    throughput.SendOnewayByteSeq(byteSeq);
                                    break;
                                }
                                case 'r':
                                {
                                    throughput.RecvByteSeq();
                                    break;
                                }
                                case 'e':
                                {
                                    throughput.EchoByteSeq(byteSeq);
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
                                    throughput.SendStringSeq(stringSeq);
                                    break;
                                }
                                case 'o':
                                {
                                    throughput.SendOnewayStringSeq(stringSeq);
                                    break;
                                }
                                case 'r':
                                {
                                    throughput.RecvStringSeq();
                                    break;
                                }
                                case 'e':
                                {
                                    throughput.EchoStringSeq(stringSeq);
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
                                    throughput.SendStructSeq(structSeq);
                                    break;
                                }
                                case 'o':
                                {
                                    throughput.SendOnewayStructSeq(structSeq);
                                    break;
                                }
                                case 'r':
                                {
                                    throughput.RecvStructSeq();
                                    break;
                                }
                                case 'e':
                                {
                                    throughput.EchoStructSeq(structSeq);
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
                                    throughput.SendFixedSeq(fixedSeq);
                                    break;
                                }
                                case 'o':
                                {
                                    throughput.SendOnewayFixedSeq(fixedSeq);
                                    break;
                                }
                                case 'r':
                                {
                                    throughput.RecvFixedSeq();
                                    break;
                                }
                                case 'e':
                                {
                                    throughput.EchoFixedSeq(fixedSeq);
                                    break;
                                }
                            }
                            break;
                        }
                    }
                }
                watch.Stop();

                double dmsec = watch.ElapsedMilliseconds;
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
                        wireSize = structSeq[0].S.Length;
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
            else if (line == "s")
            {
                throughput.Shutdown();
            }
            else if (line == "x")
            {
                // Nothing to do
            }
            else if (line == "?")
            {
                Menu();
            }
            else
            {
                Console.WriteLine($"unknown command `{line}'");
                Menu();
            }
        }
        catch (Exception ex)
        {
            Console.Error.WriteLine(ex);
        }
    }
    while (line != "x");

    return 0;
}

static void Menu()
{
    Console.WriteLine(
@"usage:

    toggle type of data to send:
    1: sequence of bytes (default)
    2: sequence of strings 'hello'
    3: sequence of structs with a string 'hello' and a double
    4: sequence of structs with two ints and a double

    select test to run:
    t: Send sequence as twoway
    o: Send sequence as oneway
    r: Receive sequence
    e: Echo (send and receive) sequence

    other commands
    s: shutdown server
    x: exit
    ?: help
");
}
