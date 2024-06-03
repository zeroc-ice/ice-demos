// Copyright (c) ZeroC, Inc.

using Demo;

internal class Client
{
    public static int Main(string[] args)
    {
        int status = 0;

        try
        {
            using Ice.Communicator communicator = Ice.Util.initialize(ref args, "config.client");
            if (args.Length > 0)
            {
                Console.WriteLine("too many arguments");
                status = 1;
            }
            else
            {
                // await in modern C#
                status = RunAsync(communicator).GetAwaiter().GetResult();
            }
        }
        catch (Exception exception)
        {
            Console.WriteLine(exception);
            status = 1;
        }

        return status;
    }

    private static async Task<int> RunAsync(Ice.Communicator communicator)
    {
        CalculatorPrx calculator = CalculatorPrxHelper.uncheckedCast(communicator.propertyToProxy("Calculator.Proxy"));

        // Calculate 10 - 4 with an asynchronous call which in this case returns a Task<int>
        int intResult = await calculator.subtractAsync(10, 4);
        Console.WriteLine($"10 minus 4 is {intResult}");

        // Calculate 13 / 5 asynchronously
        Calculator_DivideResult divideResult = await calculator.divideAsync(13, 5);
        Console.WriteLine($"13 / 5 is {divideResult.returnValue} with a remainder of {divideResult.remainder}");

        // Same with 13 / 0
        try
        {
            divideResult = await calculator.divideAsync(13, 0);
            Console.WriteLine("13 / 0 worked!");
        }
        catch (Demo.DivideByZeroException)
        {
            Console.WriteLine("You cannot divide by 0");
        }

        // Have the calculator find the hypotenuse of a triangle with side lengths of 6 and 8
        // using the Pythagorean theorem.

        int[] squares = await Task.WhenAll(calculator.squareAsync(6), calculator.squareAsync(8));
        double hypotenuse = await calculator.squareRootAsync(await calculator.addAsync(squares[0], squares[1]));
        Console.WriteLine($"The hypotenuse of a triangle with side lengths of 6 and 8 is {hypotenuse}");

        await calculator.shutdownAsync();
        return 0;
    }
}
