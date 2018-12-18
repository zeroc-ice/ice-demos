// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef CALCULATOR_I_H_
#define CALCULATOR_I_H_

#include <Calculator.h>

class CalculatorI : public Demo::Calculator
{
public:

    virtual int add(int, int, const Ice::Current&) override;
    virtual int subtract(int, int, const Ice::Current&) override;
    virtual int divide(int, int, int&, const Ice::Current&) override;
    virtual int square(int, const Ice::Current&) override;
    virtual double squareRoot(int, const Ice::Current&) override;
    virtual void shutdown(const Ice::Current&) override;
};

#endif
