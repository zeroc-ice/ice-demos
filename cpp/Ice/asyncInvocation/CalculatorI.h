// Copyright (c) ZeroC, Inc.

#ifndef CALCULATOR_I_H_
#define CALCULATOR_I_H_

#include <Calculator.h>

class CalculatorI : public Demo::Calculator
{
public:
    int add(int, int, const Ice::Current&) override;
    int subtract(int, int, const Ice::Current&) override;
    int divide(int, int, int&, const Ice::Current&) override;
    int square(int, const Ice::Current&) override;
    double squareRoot(int, const Ice::Current&) override;
    void shutdown(const Ice::Current&) override;
};

#endif
