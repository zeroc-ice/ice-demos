// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using System;
using System.Configuration;
using System.Threading.Tasks;
using ZeroC.Ice;

try
{
    // using statement - communicator is automatically destroyed at the end of this statement
    using var communicator = new Communicator(ref args, ConfigurationManager.AppSettings);

    // Destroy the communicator on Ctrl+C or Ctrl+Break
    Console.CancelKeyPress += (sender, eventArgs) => communicator.DisposeAsync();

    if (args.Length > 0)
    {
        Console.Error.WriteLine("too many arguments");
        return 1;
    }

    ICalculatorPrx calculator = communicator.GetPropertyAsProxy("Calculator.Proxy", ICalculatorPrx.Factory) ??
        throw new ArgumentException("invalid proxy");

    Console.WriteLine($"10 minus 4 is {await calculator.SubtractAsync(10, 4)}");

    try
    {
        (int result, int remainder) = await calculator.DivideAsync(13, 5);
        Console.WriteLine($"13 / 5  is {result} with a remainder of {remainder}");
    }
    catch (Demo.DivideByZeroException)
    {
        Console.WriteLine("You cannot divide by 0");
    }

    try
    {
        (int result, int remainder) = await calculator.DivideAsync(13, 0);
        Console.WriteLine($"13 / 0  is {result} with a remainder of {remainder}");
    }
    catch (Demo.DivideByZeroException)
    {
        Console.WriteLine("You cannot divide by 0");
    }

    try
    {
        // This returns an array with the results from each operation
        int[] squares = await Task.WhenAll(calculator.SquareAsync(6), calculator.SquareAsync(8));
        var hypotenuse = await calculator.SquareRootAsync(await calculator.AddAsync(squares[0], squares[1]));
        Console.WriteLine($"The hypotenuse of a triangle with side lengths of 6 and 8 is {hypotenuse}");
    }
    catch (NegativeRootException)
    {
        Console.WriteLine("You Cannot take the square root of a negative number");
    }
    await calculator.ShutdownAsync();
}
catch (Exception ex)
{
    Console.Error.WriteLine(ex);
    return 1;
}
return 0;
