// Copyright (c) ZeroC, Inc.

#ifndef TERMINATE_H
#define TERMINATE_H

#include <Ice/Ice.h>
#include <cstdlib>
#include <exception>
#include <iostream>

namespace Terminate
{
    /// Custom terminate handler that prints unhandled exceptions to std::cerr.
    /// This function is primarily useful on Windows where the default terminate handler does not provide any
    /// useful information.
    inline void customTerminateHandler()
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
                std::cerr << "std::terminate called without an active exception" << std::endl;
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
            std::cerr << "Unknown exception caught" << std::endl;
        }
        std::abort();
    }
}

#endif
