// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

//
// This is a per-session library object.
//
class LibraryI implements Library
{
    @Override
    public Library.QueryByIsbnResult queryByIsbn(String isbn, int n, com.zeroc.Ice.Current current)
    {
        SQLRequestContext context = SQLRequestContext.getCurrentContext();
        assert context != null;

        reapQueries();

        Library.QueryByIsbnResult r = new Library.QueryByIsbnResult();

        try
        {
            java.sql.PreparedStatement stmt = context.prepareStatement("SELECT COUNT(*) FROM books WHERE isbn LIKE ?");
            stmt.setString(1, isbn + "%");
            java.sql.ResultSet rs = stmt.executeQuery();
            boolean next = rs.next();
            assert next;
            r.nrows = rs.getInt(1);
            if(r.nrows == 0)
            {
                return r;
            }

            stmt = context.prepareStatement("SELECT * FROM books WHERE isbn LIKE ?");
            stmt.setString(1, isbn + "%");
            rs = stmt.executeQuery();
            next = rs.next();
            assert next;

            r.first = new java.util.LinkedList<>();
            next = true;
            for(int i = 0; i < n && next; ++i)
            {
                r.first.add(BookI.extractDescription(context, rs, current.adapter));
                next = rs.next();
            }
            if(next)
            {
                BookQueryResultI impl = new BookQueryResultI(context, rs, current.adapter);
                r.result = BookQueryResultPrx.uncheckedCast(current.adapter.addWithUUID(impl));
                add(r.result, impl);
            }
        }
        catch(java.sql.SQLException e)
        {
            JDBCException ex = new JDBCException();
            ex.initCause(e);
            throw ex;
        }

        return r;
    }

    @Override
    public Library.QueryByAuthorResult queryByAuthor(String author, int n, com.zeroc.Ice.Current current)
    {
        SQLRequestContext context = SQLRequestContext.getCurrentContext();
        assert context != null;

        reapQueries();

        Library.QueryByAuthorResult r = new Library.QueryByAuthorResult();

        try
        {
            // Find each of the authors.
            java.sql.PreparedStatement stmt = context.prepareStatement("SELECT * FROM authors WHERE name LIKE ?");
            stmt.setString(1, "%" + author + "%");
            java.sql.ResultSet rs = stmt.executeQuery();
            if(!rs.next())
            {
                // No results are available.
                r.nrows = 0;
                return r;
            }

            // Build a query that finds all books by these authors.
            StringBuilder sb = new StringBuilder(128);
            sb.append("(");
            boolean front = true;
            do
            {
                if(!front)
                {
                    sb.append(" OR ");
                }
                front = false;
                sb.append("authors_books.author_id=");
                sb.append(rs.getInt("id"));
            }
            while(rs.next());
            sb.append(")");

            stmt = context.prepareStatement(
                "SELECT COUNT(DISTINCT ID) FROM books INNER JOIN authors_books ON books.id=authors_books.book_id AND "
                + sb.toString());
            rs = stmt.executeQuery();
            boolean next = rs.next();
            assert next;
            r.nrows = rs.getInt(1);
            if(r.nrows == 0)
            {
                return r;
            }

            // Execute the query.
            stmt = context.prepareStatement(
                "SELECT DISTINCT ID, ISBN, TITLE, RENTER_ID FROM books INNER JOIN authors_books ON " +
                "books.id=authors_books.book_id AND " + sb.toString());
            rs = stmt.executeQuery();
            next = rs.next();
            assert next;

            next = true;
            r.first = new java.util.LinkedList<>();
            for(int i = 0; i < n && next; ++i)
            {
                r.first.add(BookI.extractDescription(context, rs, current.adapter));
                next = rs.next();
            }

            if(next)
            {
                BookQueryResultI impl = new BookQueryResultI(context, rs, current.adapter);
                r.result = BookQueryResultPrx.uncheckedCast(current.adapter.addWithUUID(impl));
                add(r.result, impl);
            }
        }
        catch(java.sql.SQLException e)
        {
            JDBCException ex = new JDBCException();
            ex.initCause(e);
            throw ex;
        }

        return r;
    }

    @Override
    public Library.QueryByTitleResult queryByTitle(String title, int n, com.zeroc.Ice.Current current)
    {
        SQLRequestContext context = SQLRequestContext.getCurrentContext();
        assert context != null;

        reapQueries();

        Library.QueryByTitleResult r = new Library.QueryByTitleResult();

        try
        {
            java.sql.PreparedStatement stmt = context.prepareStatement("SELECT COUNT(*) FROM books WHERE title LIKE ?");
            stmt.setString(1, "%" + title + "%");
            java.sql.ResultSet rs = stmt.executeQuery();
            boolean next = rs.next();
            assert next;
            r.nrows = rs.getInt(1);
            if(r.nrows == 0)
            {
                return r;
            }

            stmt = context.prepareStatement("SELECT * FROM books WHERE title LIKE ?");
            stmt.setString(1, "%" + title + "%");
            rs = stmt.executeQuery();
            next = rs.next();
            assert next;

            r.first = new java.util.LinkedList<>();
            next = true;
            for(int i = 0; i < n && next; ++i)
            {
                r.first.add(BookI.extractDescription(context, rs, current.adapter));
                next = rs.next();
            }
            if(next)
            {
                BookQueryResultI impl = new BookQueryResultI(context, rs, current.adapter);
                r.result = BookQueryResultPrx.uncheckedCast(current.adapter.addWithUUID(impl));
                add(r.result, impl);
            }
        }
        catch(java.sql.SQLException e)
        {
            JDBCException ex = new JDBCException();
            ex.initCause(e);
            throw ex;
        }

        return r;
    }

    @Override
    public BookPrx createBook(String isbn, String title, java.util.List<String> authors, com.zeroc.Ice.Current current)
        throws BookExistsException, InvalidISBNException
    {
        SQLRequestContext context = SQLRequestContext.getCurrentContext();
        assert context != null;
        try
        {
            java.sql.PreparedStatement stmt = context.prepareStatement("SELECT * FROM books WHERE isbn = ?");
            stmt.setString(1, isbn);
            java.sql.ResultSet rs = stmt.executeQuery();
            if(rs.next())
            {
                throw new BookExistsException();
            }

            if(isbn.length() > 13)
            {
                throw new InvalidISBNException();
            }

            //
            // First convert the authors string to an id set.
            //
            java.util.List<Integer> authIds = new java.util.LinkedList<>();
            for(String author : authors)
            {
                Integer id;
                stmt = context.prepareStatement("SELECT * FROM authors WHERE name = ?");
                stmt.setString(1, author);
                rs = stmt.executeQuery();
                if(rs.next())
                {
                    // If there is a result, then the database
                    // already contains this author.
                    id = rs.getInt(1);
                    assert !rs.next();
                }
                else
                {
                    // Otherwise, create a new author record.
                    stmt = context.prepareStatement("INSERT INTO authors (name) VALUES(?)",
                                                    java.sql.Statement.RETURN_GENERATED_KEYS);
                    stmt.setString(1, author);
                    int count = stmt.executeUpdate();
                    assert count == 1;
                    rs = stmt.getGeneratedKeys();
                    boolean next = rs.next();
                    assert next;
                    id = rs.getInt(1);
                }

                // Add the new id to the list of ids.
                authIds.add(id);
            }

            // Create the new book.
            stmt = context.prepareStatement("INSERT INTO books (isbn, title) VALUES(?, ?)",
                                            java.sql.Statement.RETURN_GENERATED_KEYS);
            stmt.setString(1, isbn);
            stmt.setString(2, title);
            int count = stmt.executeUpdate();
            assert count == 1;

            rs = stmt.getGeneratedKeys();
            boolean next = rs.next();
            assert next;
            Integer bookId = rs.getInt(1);

            // Create new authors_books records.
            for(Integer i : authIds)
            {
                stmt = context.prepareStatement("INSERT INTO authors_books (book_id, author_id) VALUES(?, ?)");
                stmt.setInt(1, bookId);
                stmt.setInt(2, i);
                count = stmt.executeUpdate();
                assert count == 1;
            }

            return BookPrx.uncheckedCast(current.adapter.createProxy(BookI.createIdentity(bookId)));
        }
        catch(java.sql.SQLException e)
        {
            JDBCException ex = new JDBCException();
            ex.initCause(e);
            throw ex;
        }
    }

    LibraryI()
    {
    }

    synchronized public void destroy()
    {
        if(_destroyed)
        {
            return;
        }
        _destroyed = true;
        for(QueryProxyPair p : _queries)
        {
            try
            {
                p.proxy.destroy();
            }
            catch(com.zeroc.Ice.ObjectNotExistException e)
            {
                // Ignore, it could have already been destroyed.
            }
        }
    }

    synchronized public void shutdown()
    {
        if(_destroyed)
        {
            return;
        }
        _destroyed = true;

        // Shutdown each of the associated query objects.
        for(QueryProxyPair p : _queries)
        {
            p.impl.shutdown();
        }
    }

    synchronized private void add(BookQueryResultPrx proxy, BookQueryResultI impl)
    {
        // If the session has been destroyed, then destroy the book
        // result, and raise an ObjectNotExistException.
        if(_destroyed)
        {
            proxy.destroy();
            throw new com.zeroc.Ice.ObjectNotExistException();
        }
        _queries.add(new QueryProxyPair(proxy, impl));
    }

    synchronized private void reapQueries()
    {
        if(_destroyed)
        {
            throw new com.zeroc.Ice.ObjectNotExistException();
        }

        java.util.Iterator<QueryProxyPair> p = _queries.iterator();
        while(p.hasNext())
        {
            QueryProxyPair pair = p.next();
            try
            {
                pair.proxy.ice_ping();
            }
            catch(com.zeroc.Ice.ObjectNotExistException e)
            {
                p.remove();
            }
        }
    }

    static class QueryProxyPair
    {
        QueryProxyPair(BookQueryResultPrx p, BookQueryResultI i)
        {
            proxy = p;
            impl = i;
        }

        BookQueryResultPrx proxy;
        BookQueryResultI impl;
    }

    private java.util.List<QueryProxyPair> _queries = new java.util.LinkedList<>();
    private boolean _destroyed = false;
}
