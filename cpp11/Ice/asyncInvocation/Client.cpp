// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>

#include <tuple>

#include <Calculator.h>

using namespace std;

int run(const shared_ptr<Ice::Communicator>&, const string&);

int
main(int argc, char* argv[])
{
    int status = 0;

    try
    {
        //
        // CtrlCHandler must be created before the communicator or any other threads are started
        //
        Ice::CtrlCHandler ctrlCHandler;

        //
        // CommunicatorHolder's ctor initializes an Ice communicator,
        // and its dtor destroys this communicator.
        //
        Ice::CommunicatorHolder ich(argc, argv, "config.client");
        auto communicator = ich.communicator();

        ctrlCHandler.setCallback(
            [communicator](int)
            {
                communicator->destroy();
            });

        //
        // The communicator initialization removes all Ice-related arguments from argc/argv
        //
        if(argc > 1)
        {
            cerr << argv[0] << ": too many arguments" << endl;
            status = 1;
        }
        else
        {
            status = run(communicator, argv[0]);
        }
    }
    catch(const std::exception& ex)
    {
        cerr << ex.what() << endl;
        status = 1;
    }

    return status;
}

int
run(const shared_ptr<Ice::Communicator>& communicator, const string& appName)
{
    auto calculator = Ice::checkedCast<Demo::CalculatorPrx>(communicator->propertyToProxy("Calculator.Proxy"));
    if(!calculator)
    {
        cerr << appName << ": invalid proxy" << endl;
        return 1;
    }

    // Calculate 10 - 4 with future-based async operations which return a std::future
    auto fut1 = calculator->subtractAsync(10, 4);
    cout << "10 minus 4 is " << fut1.get() << endl;

    // Calculate 13 / 5
    auto fut2 = calculator->divideAsync(13, 5);
    // Since the 'divide' operation has output parameters, the result of 'get' is a struct
    {
        auto result = fut2.get();
        cout << "13 / 5 is " << result.returnValue << " with a remainder of " << result.remainder << endl;
    }

    //same with 13 / 0, which throws an exception
    try
    {
        auto fut3 = calculator->divideAsync(13, 0);
        auto result = fut3.get();
        cout << "13 / 0 is " << result.returnValue << " with a remainder of " << result.remainder << endl;
    }
    catch(const Demo::DivideByZeroException&)
    {
        cout << "You cannot divide by 0" << endl;
    }

    // Calculate the hypotenuse of a right triangle with side lengths of 6 and 8 using the Pythagorean Theorem
    // by chaining asynchronous futures
    try
    {
        auto side1 = calculator->squareAsync(6);
        auto side2 = calculator->squareAsync(8);
        auto sum = calculator->addAsync(side1.get(), side2.get());
        auto hypotenuse = calculator->squareRootAsync(sum.get());
        cout << "The hypotenuse of a triangle with side lengths of 6 and 8 is " << hypotenuse.get() << endl;
    }
    catch(const Demo::NegativeRootException&)
    {
        cout << "You cannot take the square root of a negative number";
    }

    // Calculate 10 - 4 with callback-based async operations which takes 'std::function's as callback parameters, the first
    // of which is called when the operation is completed
    {
        promise<int> p;
        calculator->subtractAsync(10, 4,
                                    [&p](int answer)
                                    {
                                        p.set_value(answer);
                                    });
        // Wait for the operation result using a future
        cout << "10 minus 4 is " << p.get_future().get() << endl;
    }

    // Same with 13 / 0:
    {
        promise<tuple<int, int>> p;
        calculator->divideAsync(13, 0,
                                [&p](int answer, int remainder)
                                {
                                    assert(false);
                                    p.set_value(make_tuple(answer, remainder));
                                },
                                [&p](exception_ptr ex)
                                {
                                    p.set_exception(ex);
                                });
        try
        {
            auto result = p.get_future().get();
            cout << "13 / 0 is " << get<0>(result) << " with a remainder of " << get<1>(result) << endl;
        }
        catch(const Demo::DivideByZeroException&)
        {
            cout << "You cannot divide by 0" << endl;
        }
    }

    // Calculate the hypotenuse of a right triangle with side lengths of 6 and 8 using the Pythagorean Theorem
    // and chained callbacks
    promise<int> a2;
    calculator->squareAsync(6,
        [&a2](int answer)
        {
            a2.set_value(answer);
        });

    promise<int> b2;
    calculator->squareAsync(8,
        [&b2](int answer)
        {
            b2.set_value(answer);
        });

    promise<double> hypotenuse;
    calculator->addAsync(a2.get_future().get(), b2.get_future().get(),
        [&calculator, &hypotenuse](int addResponse)
        {
            calculator->squareRootAsync(addResponse,
                [&hypotenuse](double squareRootResponse)
                {
                    hypotenuse.set_value(squareRootResponse);
                });
        });
    cout << "The hypotenuse of a triangle with side lengths of 6 and 8 is " << hypotenuse.get_future().get() << endl;

    calculator->shutdown();
    return 0;
}
