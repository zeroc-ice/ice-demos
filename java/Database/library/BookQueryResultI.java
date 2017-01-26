// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

class BookQueryResultI implements BookQueryResult
{
    BookQueryResultI(SQLRequestContext context, java.sql.ResultSet rs, com.zeroc.Ice.ObjectAdapter adapter)
        throws java.sql.SQLException
    {
        _books = new java.util.Stack<>();
        for(int i = 0; i < MAX_BOOK_QUERY_RESULT; ++i)
        {
            _books.add(BookI.extractDescription(context, rs, adapter));
            if(!rs.next())
            {
                break;
            }
        }
    }

    @Override
    synchronized public BookQueryResult.NextResult next(int n, com.zeroc.Ice.Current current)
    {
        if(_destroyed)
        {
            throw new com.zeroc.Ice.ObjectNotExistException();
        }
        BookQueryResult.NextResult r = new BookQueryResult.NextResult();
        r.destroyed = false;
        r.returnValue = new java.util.LinkedList<>();
        if(n <= 0)
        {
            return r;
        }

        for(int i = 0; i < n && _books.size() > 0; ++i)
        {
            r.returnValue.add(_books.pop());
        }

        if(_books.size() <= 0)
        {
            try
            {
                r.destroyed = true;
                destroy(current);
            }
            catch(Exception e)
            {
                // Ignore.
            }
        }

        return r;
    }

    @Override
    synchronized public void destroy(com.zeroc.Ice.Current current)
    {
        if(_destroyed)
        {
            throw new com.zeroc.Ice.ObjectNotExistException();
        }
        _destroyed = true;

        current.adapter.remove(current.id);
    }

    // Called on application shutdown by the Library.
    synchronized public void shutdown()
    {
        if(!_destroyed)
        {
            _destroyed = true;
        }
    }

    private java.util.Stack<BookDescription> _books;
    private boolean _destroyed = false;
    private static final int MAX_BOOK_QUERY_RESULT = 1000;
}
