// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Calculator.h>

using namespace std;
using namespace Demo;

class Client : public Ice::Application
{
public:

    virtual int run(int, char*[]) override;
};

int
main(int argc, char* argv[])
{
    Client app;
    return app.main(argc, argv, "config.client");
}

int
Client::run(int argc, char* argv[])
{
    if(argc > 1)
    {
        cerr << appName() << ": too many arguments" << endl;
        return EXIT_FAILURE;
    }

    auto calculator = Ice::checkedCast<CalculatorPrx>(communicator()->propertyToProxy("Calculator.Proxy"));
    if(!calculator)
    {
        cerr << argv[0] << ": invalid proxy" << endl;
        return EXIT_FAILURE;
    }

    // async substraction returns a std::future
    auto fut1 = calculator->subtractAsync(10, 4);
    cout << "10 minus 4 is " << fut1.get() << endl;

    // async division that returns a std::future
    auto fut2 = calculator->divideAsync(13, 5);
    // Since 'divide' has output parameters, first retrieve the result, which is a struct
    auto result = fut2.get();
    cout << "13 / 5 is " << result.returnValue << " with a remainder of " << result.remainder << endl;

    // Have the calculator find the hypotenuse of a triangle with side lengths of 6 and 8 using the Pythagorean theorem
    // and future based asynchronous invocation
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

    // Have the calculator compute 10 - 4 using callback based asynchronous invocation; the result is given to
    // the response function parameter after the in parameters:
    calculator->subtractAsync(10, 4,
                              [](int answer) { cout << "10 - 4 is " << answer << endl; });

    // Have the calculator compute 13 / 5 using callback based asynchronous invocation, by passing in a response
    // function and an exception function after the in parameters:
    calculator->divideAsync(13, 5,
                            [](int answer, int remainder)
                            {
                                cout << "13 / 5 is " << answer << " with a remainder of " << remainder << endl;
                            },
                            [](exception_ptr eptr)
                            {
                                try
                                {
                                    rethrow_exception(eptr);
                                }
                                catch(const std::exception& ex)
                                {
                                    cerr << "Request failed: " << ex.what() << endl;
                                }
                            });

    // Same with 13 / 0:
    calculator->divideAsync(13, 0,
                            [](int answer, int remainder)
                            {
                                cout << "13 / 0 is " << answer << " with a remainder of " << remainder << endl;
                            },
                            [](exception_ptr eptr)
                            {
                                try
                                {
                                    rethrow_exception(eptr);
                                }
                                catch(const std::exception& ex)
                                {
                                    cout << "13 / 0 raised: " << ex.what() << endl;
                                }
                            });

    calculator->shutdown();
    return EXIT_SUCCESS;
}
