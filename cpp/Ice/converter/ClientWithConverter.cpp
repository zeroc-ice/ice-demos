// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Client.h>
#include <StringConverterI.h>

using namespace std;
using namespace Demo;

int
#ifdef _WIN32
wmain(int argc, wchar_t* argv[])
#else
main(int argc, char* argv[])
#endif
{
    int status = EXIT_SUCCESS;
    try
    {
        //
        // Set the process string converter and then initialize the
        // application.
        //
        setProcessStringConverter(new StringConverterI());
        Client app;
        status = app.main(argc, argv, "config.client");
    }
    catch(const Ice::Exception& ex)
    {
        cerr << ex << endl;
        status = EXIT_FAILURE;
    }
    return status;
}
