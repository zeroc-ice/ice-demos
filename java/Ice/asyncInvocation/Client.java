// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import java.util.concurrent.CompletableFuture;
import java.util.concurrent.CompletionException;
import java.util.concurrent.ExecutionException;
import Demo.*;

public class Client extends com.zeroc.Ice.Application
{
    @Override
    public int run(String[] args)
    {
        if(args.length > 0)
        {
            System.err.println(appName() + ": too many arguments");
            return 1;
        }

        CalculatorPrx calculator = CalculatorPrx.checkedCast(communicator().propertyToProxy("Calculator.Proxy"));
        if(calculator == null)
        {
            System.err.println("invalid proxy");
            return 1;
        }

        // Calculate 10 - 4 with an asynchronous call
        try
        {
            CompletableFuture<Integer> result = calculator.subtractAsync(10, 4);

            // 'get' blocks until the call is completed
            System.out.println("10 minus 4 is " + result.get());
        }
        catch(ExecutionException | InterruptedException exception)
        {
            exception.printStackTrace();
        }

        // Calculate 13 / 5 asynchronously using a lambda to process the result
        try
        {
            calculator.divideAsync(13, 5).whenComplete(
                (completed, exception) ->
                {
                    if(exception != null)
                    {
                        if(exception instanceof DivideByZeroException)
                        {
                            System.out.println("You cannot divide by 0");
                        }
                        else
                        {
                            exception.printStackTrace();
                        }
                    }
                    else
                    {
                        System.out.println("13 / 5 is " + completed.returnValue + " with a remainder of " + completed.remainder);
                    }
                }).join();
        }
        catch(CompletionException e)
        {
            //ignored
        }

        // Same with divide by 0:
        try
        {
            calculator.divideAsync(13, 0).whenComplete(
                (completed, exception) ->
                {
                    if(exception != null)
                    {
                        if(exception instanceof DivideByZeroException)
                        {
                            System.out.println("You cannot divide by 0");
                        }
                        else
                        {
                            exception.printStackTrace();
                        }
                    }
                    else
                    {
                        System.out.println("13 / 0 is " + completed.returnValue + " with a remainder of " + completed.remainder);
                    }
                }).join();
        }
        catch(CompletionException e)
        {
            //ignored
        }

        // Have the calculator find the hypotenuse of a triangle with side lengths of 6 and 8
        // using the Pythagorean theorem and chaining async calls
        try
        {
            CompletableFuture<Integer> side1 = calculator.squareAsync(6);
            CompletableFuture<Integer> side2 = calculator.squareAsync(8);

            double h = CompletableFuture.allOf(side1, side2).thenComposeAsync((result) ->
                {
                    try
                    {
                        return calculator.addAsync(side1.get(), side2.get());
                    }
                    catch(Exception ex)
                    {
                        throw new CompletionException(ex);
                    }
                }
            ).thenComposeAsync((result) ->
                {
                    return calculator.squareRootAsync(result);
                }
            ).handle((result, throwable) ->
                {
                    return result;
                }).get();
            
            System.out.println("The hypotenuse of a triangle with side lengths of 6 and 8 is " + h);
        }
        catch(ExecutionException ex)
        {
            if(ex.getCause() instanceof NegativeRootException)
            {
                System.out.println("You cannot take the square root of negative numbers");
            }
            else
            {
                ex.printStackTrace();
            }
        }
        catch(InterruptedException ex)
        {
        }

        calculator.shutdown();
        return 0;
    }

    public static void main(String[] args)
    {
        Client app = new Client();
        int status = app.main("Client", args, "config.client");
        System.exit(status);
    }
}
