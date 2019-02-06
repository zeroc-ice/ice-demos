//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.Ice.Current;

import com.zeroc.demos.Ice.asyncInvocation.Demo.*;

public class CalculatorI implements Calculator
{
    @Override
    public int add(int x, int y, Current current)
    {
        return x + y;
    }

    @Override
    public int subtract(int x, int subtrahend, Current current)
    {
        return x - subtrahend;
    }

    @Override
    public DivideResult divide(int numerator, int denominator, Current current) throws DivideByZeroException
    {
        if(denominator == 0)
        {
            throw new DivideByZeroException();
        }
        return new DivideResult(numerator / denominator, numerator % denominator);
    }

    @Override
    public int square(int x, Current current)
    {
        return x * x;
    }

    @Override
    public double squareRoot(int x, Current current) throws NegativeRootException
    {
        if(x < 0)
        {
            throw new NegativeRootException();
        }
        return Math.sqrt(x);
    }

    @Override
    public void shutdown(Current current)
    {
        current.adapter.getCommunicator().shutdown();
    }
}
