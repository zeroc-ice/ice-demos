// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Parser.h>

using namespace std;
using namespace Demo;

int
runParser(int, char* argv[], const Ice::CommunicatorPtr& communicator)
{
    PhoneBookPrx phoneBook = PhoneBookPrx::checkedCast(communicator->propertyToProxy("PhoneBook.Proxy"));
    if(!phoneBook)
    {
        cerr << argv[0] << ": invalid proxy" << endl;
        return EXIT_FAILURE;
    }

    ParserPtr p = new Parser(phoneBook);
    return p->parse();
}
