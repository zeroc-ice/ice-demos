// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Parser.h>

using namespace std;
using namespace Demo;

int
runParser(int, char* argv[], const Ice::CommunicatorPtr& communicator)
{
    LibraryPrx library = LibraryPrx::checkedCast(communicator->propertyToProxy("Library.Proxy"));
    if(!library)
    {
        cerr << argv[0] << ": invalid proxy" << endl;
        return EXIT_FAILURE;
    }

    ParserPtr p = new Parser(library);
    return p->parse();
}
