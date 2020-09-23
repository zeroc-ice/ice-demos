// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using ZeroC.Ice;

namespace Demo
{
    public class Calculator : ICalculator
    {
        public int Add(int x, int y, Current current) => x + y;

        public int Subtract(int x, int subtrahend, Current current) => x - subtrahend;

        public (int ReturnValue, int Remainder) Divide(int numerator, int denominator, Current current)
        {
            if (denominator == 0)
            {
                throw new DivideByZeroException();
            }
            return (numerator / denominator, numerator % denominator);
        }

        public int Square(int x, Current current) => x * x;

        public double SquareRoot(int x, Current current)
        {
            if (x < 0)
            {
                throw new NegativeRootException();
            }
            return Math.Sqrt(x);
        }

        public void Shutdown(Current current) => current.Adapter.Communicator.DisposeAsync();
    }

}
