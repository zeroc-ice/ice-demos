// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

module Demo
{

// Indicates that a division by 0 was attempted
exception DivideByZeroException
{
}

// Indicates that the square root of a negative number was attempted
exception NegativeRootException
{
}

// The calculator interface with all the mathematical functions it supports
interface Calculator
{
    // Adds two integers x and y together and returns the result
    idempotent int add(int x, int y);

    // Subtracts the subtrahend from x and returns the result
    idempotent int subtract(int x, int subtrahend);

    // Takes two parameters, a numerator and a denominator and returns the result of performing integer division of
    // the numerator divided by the denominator; as well as the integer remainder as an output parameter.
    // Throws a DivideByZeroException if denominator = 0
    idempotent int divide(int numerator, int denominator, out int remainder) throws DivideByZeroException;

    // Multiplies x by itself, and returns the result
    idempotent int square(int x);

    // Calculates the square root of x, and returns the resulting double
    // Throws NegativeRootException if x < 0
    idempotent double squareRoot(int x) throws NegativeRootException;

    // Shuts down the calculator adapter
    void shutdown();
}

}
