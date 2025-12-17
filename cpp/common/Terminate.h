// Copyright (c) ZeroC, Inc.

#ifndef TERMINATE_H
#define TERMINATE_H

#include <Ice/Ice.h>
#include <cstdlib>
#include <exception>
#include <iostream>

namespace Terminate
{
    /// Custom terminate handler that prints unhandled exceptions to std::cerr before aborting the program.
    /// This custom handler is primarily useful on Windows where the default terminate handler aborts the program
    /// without providing any information.
    inline void printCurrentException()
    {
        try
        {
            std::exception_ptr eptr{std::current_exception()};
            if (eptr)
            {
                std::rethrow_exception(eptr);
            }
            else
            {
                std::cerr << "std::terminate called without a current exception" << std::endl;
            }
        }
        catch (const Ice::Exception& ex)
        {
            // We can stream Ice exceptions with operator<<.
            std::cerr << ex << std::endl;
        }
        catch (const std::exception& ex)
        {
            std::cerr << "std::exception: " << ex.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "Caught unknown exception" << std::endl;
        }
        std::abort();
    }
}

#endif
