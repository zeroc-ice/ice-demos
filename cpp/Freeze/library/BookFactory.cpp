// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <BookFactory.h>

using namespace std;

BookFactory::BookFactory(const LibraryIPtr& library) :
    _library(library)
{
}

Ice::ObjectPtr
#ifndef NDEBUG
BookFactory::create(const string& type)
#else
BookFactory::create(const string&)
#endif
{
    assert(_library);
    assert(type == "::Demo::Book");
    return new BookI(_library);
}

void
BookFactory::destroy()
{
    //
    // Break cyclic object dependencies
    //
    _library = 0;
}
