// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Demo;
using System;
using System.Threading.Tasks;

public class Client
{
    public static int Main(string[] args)
    {
        int status = 0;

        try
        {
            //
            // using statement - communicator is automatically destroyed
            // at the end of this statement
            //
            using(var communicator = Ice.Util.initialize(ref args, "config.client"))
            {
                //
                // Destroy the communicator on Ctrl+C or Ctrl+Break
                //
                Console.CancelKeyPress += (sender, eventArgs) => communicator.destroy();

                if(args.Length > 0)
                {
                    Console.Error.WriteLine("too many arguments");
                    status = 1;
                }
                else
                {
                    status = run(communicator);
                }
            }
        }
        catch(Exception ex)
        {
            Console.Error.WriteLine(ex);
            status = 1;
        }

        return status;
    }

    private static int run(Ice.Communicator communicator)
    {
        var calculator = CalculatorPrxHelper.checkedCast(communicator.propertyToProxy("Calculator.Proxy"));
        if(calculator == null)
        {
            Console.Error.WriteLine("invalid proxy");
            return 1;
        }

        // Calculate 10 - 4 with an asynchronous call which in this case returns a Task<int>
        Console.WriteLine("10 minus 4 is {0}", calculator.subtractAsync(10, 4).Result);

        // Calculate 13 / 5 asynchronously, and continue it with a lambda that is run when 'divideAsync' completes
        calculator.divideAsync(13, 5).ContinueWith(t =>
        {
            try
            {
                // Since the divide operation has output parameters, 'Result' gives a DivideResult object here
                Console.WriteLine("13 / 5 is {0} with a remainder of {1}",
                                  t.Result.returnValue, t.Result.remainder);
            }
            catch(AggregateException ex)
            {
                ex.Handle((e) =>
                {
                    if(e is Demo.DivideByZeroException)
                    {
                        Console.WriteLine("You cannot divide by zero");
                        // Don't let this exception stop the rest of the program's execution
                        return true;
                    }
                    else
                    {
                        Console.WriteLine("Request failed: {0}", e.Message);
                        // Allow this exception to terminate the program's execution
                        return false;
                    }
                });
            }
        }).Wait();

        // Same with 13 / 0
        calculator.divideAsync(13, 0).ContinueWith(t =>
        {
            try
            {
                Console.WriteLine("13 / 0 is {0} with a remainder of {1}",
                                  t.Result.returnValue, t.Result.remainder);
            }
            catch(AggregateException ex)
            {
                ex.Handle((e) =>
                {
                    if(e is Demo.DivideByZeroException)
                    {
                        Console.WriteLine("You cannot divide by 0");
                        return true;
                    }
                    else
                    {
                        Console.WriteLine("Request failed: {0}", e.Message);
                        return false;
                    }
                });
            }
        }).Wait();

        //
        // Have the calculator find the hypotenuse of a triangle with side lengths of 6 and 8
        // using the Pythagorean theorem and chained asynchronous tasks.
        //
        try
        {
            var h = Task.WhenAll(calculator.squareAsync(6), calculator.squareAsync(8)).ContinueWith(
                t => calculator.addAsync(t.Result[0], t.Result[1])).Unwrap().ContinueWith(
                    t => calculator.squareRootAsync(t.Result)).Unwrap().Result;

            Console.WriteLine("The hypotenuse of a triangle with side lengths of 6 and 8 is {0}", h);
        }
        catch(NegativeRootException)
        {
            Console.WriteLine("You cannot take the square root of a negative number");
        }

        //
        // Do the same calculations using async functions and await
        //
        Task.Run(async() =>
        {
            Console.WriteLine("10 minus 4 is {0}", await calculator.subtractAsync(10, 4));

            try
            {
                var r = await calculator.divideAsync(13, 5);
                Console.WriteLine("13 / 5  is {0} with a remainder of {1}", r.returnValue, r.remainder);
            }
            catch(Demo.DivideByZeroException)
            {
                Console.WriteLine("You cannot divide by 0");
            }

            try
            {
                var r = await calculator.divideAsync(13, 0);
                Console.WriteLine("13 / 0  is {0} with a remainder of {1}", r.returnValue, r.remainder);
            }
            catch(Demo.DivideByZeroException)
            {
                Console.WriteLine("You cannot divide by 0");
            }

            try
            {
                // This returns an array with the results from each operation
                var squares = await Task.WhenAll(calculator.squareAsync(6), calculator.squareAsync(8));
                var h = await calculator.squareRootAsync(await calculator.addAsync(squares[0], squares[1]));
                Console.WriteLine("The hypotenuse of a triangle with side lengths of 6 and 8 is {0}", h);
            }
            catch(NegativeRootException)
            {
                Console.WriteLine("You Cannot take the square root of a negative number");
            }

        }).Wait();

        calculator.shutdown();

        return 0;
    }
}
