// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <mutex>
#include <condition_variable>
#include <Ice/Ice.h>
#include "Calculator.h"

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

    // Calculate 10 - 4 with future-based async operations which return a std::future
    auto fut1 = calculator->subtractAsync(10, 4);
    cout << "10 minus 4 is " << fut1.get() << endl;

    // Calculate 13 / 5
    auto fut2 = calculator->divideAsync(13, 5);
    // Since the 'divide' operation has output parameters, the result of 'get' is a struct
    auto result = fut2.get();
    cout << "13 / 5 is " << result.returnValue << " with a remainder of " << result.remainder << endl;

    //same with 13 / 0, which throw an exception
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

    mutex mx;
    condition_variable cv;
    bool finished = false;

    // Calculate 10 - 4 with callback-based async operations which take 'std::function's as callback parameters, the first
    // of which gets called when the operation is completed
    calculator->subtractAsync(10, 4,
                                [&mx, &cv, &finished](int answer)
                                {
                                    cout << "10 minus 4 is " << answer << endl;
                                    //notify that the operation has finished
                                    lock_guard<mutex> lock(mx);
                                    finished = true;
                                    cv.notify_one();
                                });
    //wait to ensure the operation has completed
    unique_lock<mutex> lock(mx);
    cv.wait(lock, [&finished] { return finished; });

    finished = false;
    // Same with 13 / 0:
    calculator->divideAsync(13, 0,
                            [&mx, &cv, &finished](int answer, int remainder)
                            {
                                cout << "13 / 0 is " << answer << " with a remainder of " << remainder << endl;

                                lock_guard<mutex> lock(mx);
                                finished = true;
                                cv.notify_one();
                            },
                            [&mx, &cv, &finished](exception_ptr eptr)
                            {
                                try
                                {
                                    rethrow_exception(eptr);
                                }
                                catch (const Demo::DivideByZeroException&)
                                {
                                    cout << "You cannot divide by 0" << endl;
                                }

                                lock_guard<mutex> lock(mx);
                                finished = true;
                                cv.notify_one();
                            });
    cv.wait(lock, [&finished] { return finished; });
    
    
    class HypotenuseHelper
    {
    public:
        HypotenuseHelper(const shared_ptr<Demo::CalculatorPrx> calculator) : _calculator(calculator){}

        void findHypotenuse(int x, int y)
        {
            _finished = false;
            _otherOperationFinished = false;
            _calculator->squareAsync(x,
                                    [this](int answer)
                                    {
                                        this->afterSquared(answer);
                                    });
            _calculator->squareAsync(y,
                                    [this](int answer)
                                    {
                                        this->afterSquared(answer);
                                    });
        }

        double getHypotenuse()
        {
            unique_lock<mutex> lock(_hypotenuseMx);
            _hypotenuseCv.wait(lock, [this] { return this->_finished; });
            return _hypotenuse;
        }

    private:
        shared_ptr<Demo::CalculatorPrx> _calculator;
        mutex _hypotenuseMx;
        condition_variable _hypotenuseCv;
        bool _otherOperationFinished;
        int _otherAnswer;
        double _hypotenuse;
        bool _finished;

        void afterSquared(int answer)
        {
            lock_guard<mutex> lock(_hypotenuseMx);
            if(_otherOperationFinished)
            {
                _calculator->addAsync(answer, _otherAnswer,
                                    [this](int sum)
                                    {
                                        this->afterSum(sum);
                                    });
            }
            else
            {
                _otherOperationFinished = true;
                _otherAnswer = answer;
            }
        }

        void afterSum(int sum)
        {
            _calculator->squareRootAsync(sum,
                                        [this](double hypotenuse)
                                        {
                                            this->finished(hypotenuse);
                                        });
        }

        void finished(double hypotenuse)
        {
            _otherOperationFinished = false;
            _hypotenuse = hypotenuse;
            _finished = true;
            lock_guard<mutex> lock(_hypotenuseMx);
            _hypotenuseCv.notify_one();
        }
    };
    
    // Calculate the hypotenuse of a right triangle with side lengths of 6 and 8 using the Pythagorean Theorem
    // and chained callbacks in the HypotenuseHelper class
    HypotenuseHelper helper(calculator);
    helper.findHypotenuse(6, 8);
    auto hypotenuseAnswer = helper.getHypotenuse();
    cout << "The hypotenuse of a triangle with side lengths of 6 and 8 is " << hypotenuseAnswer << endl;

    calculator->shutdown();
    return EXIT_SUCCESS;
}