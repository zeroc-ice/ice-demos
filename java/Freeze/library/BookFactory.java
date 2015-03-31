// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

class BookFactory implements Ice.ObjectFactory
{
    @Override
    public Ice.Object
    create(String type)
    {
        assert(type.equals("::Demo::Book"));
        return new BookI(_library);
    }

    @Override
    public void
    destroy()
    {
    }

    BookFactory(LibraryI library)
    {
        _library = library;
    }

    private LibraryI _library;
}
