// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using System.Threading;
using ZeroC.Ice;

namespace Demo
{
    public class Calculator : ICalculator
    {
        public int Add(int x, int y, Current current, CancellationToken cancel) => x + y;

        public int Subtract(int x, int subtrahend, Current current, CancellationToken cancel) => x - subtrahend;

        public (int ReturnValue, int Remainder) Divide(int numerator, int denominator, Current current, CancellationToken cancel)
        {
            if (denominator == 0)
            {
                throw new DivideByZeroException();
            }
            return (numerator / denominator, numerator % denominator);
        }

        public int Square(int x, Current current, CancellationToken cancel) => x * x;

        public double SquareRoot(int x, Current current, CancellationToken cancel)
        {
            if (x < 0)
            {
                throw new NegativeRootException();
            }
            return Math.Sqrt(x);
        }

        public void Shutdown(Current current, CancellationToken cancel) => current.Adapter.Communicator.DestroyAsync();
    }

}
