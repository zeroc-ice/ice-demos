// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using System;
using System.Collections.Generic;
using System.Configuration;
using ZeroC.Ice;

// The new communicator is automatically destroyed (disposed) at the end of the using statement
using var communicator = new Communicator(ref args, ConfigurationManager.AppSettings);

// The communicator initialization removes all Ice-related arguments from args
if (args.Length > 0)
{
    throw new ArgumentException("too many arguments");
}

IPrinterPrx prx = communicator.GetPropertyAsProxy("Printer.Proxy", IPrinterPrx.Factory) ??
    throw new ArgumentException("invalid proxy");

Menu();

string? line = null;
do
{
    Console.Write("==> ");
    Console.Out.Flush();
    line = Console.In.ReadLine();
    if (line == null)
    {
        break;
    }

    if (line == "1")
    {
        // Create the request
        var request = OutgoingRequestFrame.WithParamList(
            prx,
            "printString",
            false,
            false,
            FormatType.Compact,
            context: null,
            "The streaming API works!",
            (ostr, value) => ostr.WriteString(value));

        // Invoke operation.
        IncomingResponseFrame response = prx.Invoke(request);
        if (response.ResultType == ResultType.Failure)
        {
            Console.Error.WriteLine("Unknown user exception");
        }
    }
    else if (line == "2")
    {
        // Create the request
        var request = OutgoingRequestFrame.WithParamList(
            prx,
            "printStringSequence",
            false,
            false,
            FormatType.Compact,
            context: null,
            new string[] { "The", "streaming", "API", "works!" },
            (ostr, value) => ostr.WriteSequence(value, (ostr, value) => ostr.WriteString(value)));

        // Invoke operation.
        IncomingResponseFrame response = prx.Invoke(request);
        if (response.ResultType == ResultType.Failure)
        {
            Console.Error.WriteLine("Unknown user exception");
        }
    }
    else if (line == "3")
    {
        // Create the request
        var request = OutgoingRequestFrame.WithParamList(
            prx,
            "printDictionary",
            false,
            false,
            FormatType.Compact,
            context: null,
            new Dictionary<string, string>()
            {
                { "The", "streaming" },
                { "API", "works!"}
            },
            (ostr, value) => ostr.WriteDictionary(
                value,
                (ostr, key) => ostr.WriteString(key),
                (ostr, value) => ostr.WriteString(value)));

        // Invoke operation.
        IncomingResponseFrame response = prx.Invoke(request);
        if (response.ResultType == ResultType.Failure)
        {
            Console.Error.WriteLine("Unknown user exception");
        }
    }
    else if (line == "4")
    {
        // Create the request
        var request = OutgoingRequestFrame.WithParamList(
            prx,
            "printEnum",
            false,
            false,
            FormatType.Compact,
            context: null,
            Color.green,
            (ostr, value) => ostr.Write(value));

        // Invoke operation.
        IncomingResponseFrame response = prx.Invoke(request);
        if (response.ResultType == ResultType.Failure)
        {
            Console.Error.WriteLine("Unknown user exception");
        }
    }
    else if (line == "5")
    {
        // Create the request
        var request = OutgoingRequestFrame.WithParamList(
            prx,
            "printStruct",
            false,
            false,
            FormatType.Compact,
            context: null,
            new Structure("red", Color.red),
            (ostr, value) => value.IceWrite(ostr));

        // Invoke operation.
        IncomingResponseFrame response = prx.Invoke(request);
        if (response.ResultType == ResultType.Failure)
        {
            Console.Error.WriteLine("Unknown user exception");
        }
    }
    else if (line == "6")
    {
        // Create the request
        var request = OutgoingRequestFrame.WithParamList(
            prx,
            "printStructSequence",
            false,
            false,
            FormatType.Compact,
            context: null,
            new Structure[]
            {
                new Structure("red", Color.red),
                new Structure("green", Color.green),
                new Structure("blue", Color.blue)
            },
            (ostr, value) => ostr.WriteSequence(value, (ostr, value) => value.IceWrite(ostr)));

        // Invoke operation.
        IncomingResponseFrame response = prx.Invoke(request);
        if (response.ResultType == ResultType.Failure)
        {
            Console.Error.WriteLine("Unknown user exception");
        }
    }
    else if (line == "7")
    {
        // Create the request
        var request = OutgoingRequestFrame.WithParamList(
            prx,
            "printClass",
            false,
            false,
            FormatType.Compact,
            context: null,
            new C(new Structure("blue", Color.blue)),
            (ostr, value) => ostr.WriteClass(value, C.IceTypeId));

        // Invoke operation.
        IncomingResponseFrame response = prx.Invoke(request);
        if (response.ResultType == ResultType.Failure)
        {
            Console.Error.WriteLine("Unknown user exception");
        }
    }
    else if (line == "8")
    {
        // Create the request
        var request = OutgoingRequestFrame.WithEmptyParamList(prx, "getValues", false);

        // Invoke operation.
        IncomingResponseFrame response = prx.Invoke(request);
        if (response.ResultType == ResultType.Failure)
        {
            Console.Error.WriteLine("Unknown user exception");
        }
        else
        {
            (string s, C c) = response.ReadReturnValue(
                communicator,
                istr =>
                {
                    string s = istr.ReadString();
                    C c = istr.ReadClass<C>(C.IceTypeId);
                    return (s, c);
                });
            Console.Error.WriteLine($"Got string `{s}' and class: s.Name={c.S.Name}, s.value={c.S.Value}");
        }
    }
    else if (line == "9")
    {
        var request = OutgoingRequestFrame.WithEmptyParamList(prx, "throwPrintFailure", false);
        IncomingResponseFrame response = prx.Invoke(request);

        try
        {
            response.ReadVoidReturnValue(communicator);
        }
        catch (PrintFailure)
        {
            // Expected.
        }
    }
    else if (line == "s")
    {
        prx.Invoke(OutgoingRequestFrame.WithEmptyParamList(prx, "shutdown", false));
    }
    else if (line == "x")
    {
        // Nothing to do.
    }
    else if (line == "?")
    {
        Menu();
    }
    else
    {
        Console.Error.WriteLine($"unknown command `{line}'");
        Menu();
    }
}
while (line != "x");

static void Menu()
{
    Console.WriteLine(
@"usage:
    1: print string
    2: print string sequence
    3: print dictionary
    4: print enum
    5: print struct
    6: print struct sequence
    7: print class
    8: get values
    9: throw exception
    s: shutdown server
    x: exit
    ?: help
");
}
