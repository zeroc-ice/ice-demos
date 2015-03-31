// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#ifndef BOOK_FACTORY_H
#define BOOK_FACTORY_H

#include <LibraryI.h>

class BookFactory : public Ice::ObjectFactory
{
public:

    BookFactory(const LibraryIPtr&);

    //
    // Operations from ObjectFactory
    //
    virtual Ice::ObjectPtr create(const std::string&);
    virtual void destroy();

private:

    LibraryIPtr _library;
};

#endif
