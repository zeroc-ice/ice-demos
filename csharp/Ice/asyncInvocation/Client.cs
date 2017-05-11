// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Demo;
using System;
using System.Threading.Tasks;

public class Client
{
    public class App : Ice.Application
    {
        public override int run(string[] args)
        {
            if(args.Length > 0)
            {
                Console.Error.WriteLine(appName() + ": too many arguments");
            }

            var calculator = CalculatorPrxHelper.checkedCast(communicator().propertyToProxy("Calculator.Proxy"));
            if(calculator == null)
            {
                Console.Error.WriteLine("invalid proxy");
                return 1;
            }

            // Calculate 10 - 4 with an asynchronous call which in this case returns a Task<int>
            var task1 = calculator.subtractAsync(10, 4);
            Console.WriteLine("10 minus 4 is " + task1.Result);

            // Calculate 13 / 5 asynchronously, and continue it with a lambda that is run when 'divideAsync' completes
            var task2 = calculator.divideAsync(13, 5).ContinueWith((t) =>
                {
                    try
                    {
                        // Since the divide operation has output parameters, 'Result' gives a DivideResult object here
                        var r = t.Result;
                        Console.WriteLine("13 / 5 is " + r.returnValue + " with a remainder of " + r.remainder);
                    }
                    catch (AggregateException ex)
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
                                Console.WriteLine("Request failed: " + e.Message);
                                // Allow this exception to terminate the program's execution
                                return false;
                            }
                        });
                    }
                });
            // Wait until task2 has finished completely
            task2.Wait();

            // Same with 13 / 0
            var task3 = calculator.divideAsync(13, 0).ContinueWith((t) =>
                {
                    try
                    {
                        var r = t.Result;
                        Console.WriteLine("13 / 0 is " + r.returnValue + " with a remainder of " + r.remainder);
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
                                Console.WriteLine("Request failed: " + e.Message);
                                return false;
                            }
                        });
                    }
                });
            task3.Wait();

            // Have the calculator find the hypotenuse of a triangle with side lengths of 6 and 8
            // using the Pythagorean theorem and chained asynchronous tasks through TaskFactory
            try
            {
                TaskFactory taskFactory = new TaskFactory();
                var side1 = calculator.squareAsync(6);
                var side2 = calculator.squareAsync(8);
                var sum = taskFactory.ContinueWhenAll(new Task[] { side1, side2 },
                                                tasks => calculator.addAsync(side1.Result, side2.Result));
                var hypotenuse = sum.ContinueWith(
                                                sumAnswer => calculator.squareRootAsync(sumAnswer.Result.Result));
                Console.WriteLine("The hypotenuse of a triangle with side lengths of 6 and 8 is "
                                                                                      + hypotenuse.Result.Result);
            }
            catch(NegativeRootException)
            {
                Console.WriteLine("You cannot take the square root of a negative number");
            }

            // Executes all the tasks parallel asynchronously and waits until they've all completed
            Task.WaitAll(new Task[]
                            {
                                doSubtractAsync(calculator, 10, 4),
                                doDivideAsync(calculator, 13, 5),
                                doDivideAsync(calculator, 13, 0),
                                doHypotenuseAsync(calculator, 6, 8),
                            });

            //calculator.shutdown();
            return 0;
        }

        public async Task doSubtractAsync(CalculatorPrx calculator, int x, int subtrahend)
        {
            var result = await calculator.subtractAsync(x, subtrahend);
            Console.WriteLine(x + " minus " + subtrahend + " is " + result);
        }

        public async Task doDivideAsync(CalculatorPrx calculator, int numerator, int denominator)
        {
            try
            {
                var result = await calculator.divideAsync(numerator, denominator);
                Console.WriteLine(numerator + " / " + denominator + " is " + result.returnValue + " with a remainder of " + result.remainder);
            }
            catch(Demo.DivideByZeroException)
            {
                Console.WriteLine("You cannot divide by 0");
            }
        }

        public async Task doHypotenuseAsync(CalculatorPrx calculator, int x, int y)
        {
            try
            {
                // This returns an array with the results from each operation
                var squareResult = await Task.WhenAll(calculator.squareAsync(x), calculator.squareAsync(y));
                var sumTask = calculator.addAsync(squareResult[0], squareResult[1]);
                var hypotenuseTask = calculator.squareRootAsync(await sumTask);
                Console.WriteLine("The hypotenuse of a triangle with side lengths of " + x + " and " + y + " is " + (await hypotenuseTask));
            }
            catch(Demo.NegativeRootException)
            {
                Console.WriteLine("You Cannot take the square root of a negative number");
            }
        }
    }

    public static int Main(string[] args)
    {
        var app = new App();
        return app.main(args, "config.client");
    }
}
