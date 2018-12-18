// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <CalculatorI.h>
#include <math.h>

using namespace std;

int
CalculatorI::add(int x, int y, const Ice::Current&)
{
    return x + y;
}

int
CalculatorI::subtract(int x, int subtrahend, const Ice::Current&)
{
    return x - subtrahend;
}

int
CalculatorI::divide(int numerator, int denominator, int& remainder, const Ice::Current&)
{
    if(denominator == 0)
    {
        throw Demo::DivideByZeroException();
    }
    remainder = numerator % denominator;
    return numerator / denominator;
}

int
CalculatorI::square(int x, const Ice::Current&)
{
    return x * x;
}

double
CalculatorI::squareRoot(int x, const Ice::Current&)
{
    if(x < 0)
    {
        throw Demo::NegativeRootException();
    }
    return sqrt(x);
}

void
CalculatorI::shutdown(const Ice::Current& current)
{
    current.adapter->getCommunicator()->shutdown();
}
