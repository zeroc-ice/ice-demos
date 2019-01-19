//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

package com.zeroc.library.controller;

import java.util.ArrayList;
import java.util.List;

import com.zeroc.demos.android.library.Demo.*;

import android.os.Handler;

public class QueryController
{
    public interface Listener
    {
        void onDataChange(QueryModel data, boolean saved);
        void onError();
    }

    public static final int NO_BOOK = -1;
    public static final int NEW_BOOK = -2;

    public enum QueryType
    {
        ISBN, TITLE, AUTHOR
    }

    private ArrayList<BookDescription> _books = new ArrayList<BookDescription>();
    private int _nrows = 0;
    private int _rowsQueried = 0;
    private BookQueryResultPrx _query = null;
    private Listener _listener;
    private Handler _handler;
    private LibraryPrx _library;
    private int _currentBook = NO_BOOK;
    private String _lastError;

    synchronized protected void postDataChanged(final boolean saved)
    {
        if(_listener != null)
        {
            _handler.post(new Runnable()
            {
                public void run()
                {
                    _listener.onDataChange(getQueryModel(), saved);
                }
            });
        }
    }

    synchronized protected void postError(final String string)
    {
        _lastError = string;
        if(_listener != null)
        {
            _handler.post(new Runnable()
            {
                public void run()
                {
                    _listener.onError();
                }
            });
        }
    }

    synchronized private void queryResponse(List<BookDescription> first, int nrows, BookQueryResultPrx result)
    {
        _books.clear();
        _nrows = nrows;
        _books.addAll(first);
        _query = result;
        if(_listener != null)
        {
            postDataChanged(false);
        }
    }

    synchronized private QueryModel getQueryModel()
    {
        QueryModel data = new QueryModel();
        data.books = new ArrayList<BookDescription>();
        for(BookDescription book : _books)
        {
            data.books.add((BookDescription)book.clone());
        }
        data.nrows = _nrows;
        if(_currentBook == NEW_BOOK)
        {
            data.currentBook = new BookDescription();
            data.currentBook.proxy = null;
            data.currentBook.isbn = "";
            data.currentBook.title = "";
            data.currentBook.rentedBy = "";
            data.currentBook.authors = new ArrayList<String>();
        }
        else if(_currentBook != NO_BOOK)
        {
            data.currentBook = (BookDescription)_books.get(_currentBook).clone();
        }
        return data;
    }

    // An empty query
    QueryController(Handler handler, LibraryPrx library)
    {
        _handler = handler;
        _library = library;
    }

    QueryController(Handler handler, LibraryPrx library, final Listener listener, final QueryType _type,
                    final String _queryString)
    {
        _handler = handler;
        _listener = listener;
        _library = library;

        // Send the initial data change notification.
        _listener.onDataChange(getQueryModel(), false);
        _rowsQueried = 10;

        if(_type == QueryType.ISBN)
        {
            _library.queryByIsbnAsync(_queryString, 10).whenComplete((result, ex) ->
                {
                    if(ex != null)
                    {
                        postError(ex.toString());
                    }
                    else
                    {
                        queryResponse(result.first, result.nrows, result.result);
                    }
                });

        }
        else if(_type == QueryType.AUTHOR)
        {
            _library.queryByAuthorAsync(_queryString, 10).whenComplete((result, ex) ->
                {
                    if(ex != null)
                    {
                        postError(ex.toString());
                    }
                    else
                    {
                        queryResponse(result.first, result.nrows, result.result);
                    }
                });
        }
        else
        {
            _library.queryByTitleAsync(_queryString, 10).whenComplete((result, ex) ->
                {
                    if(ex != null)
                    {
                        postError(ex.toString());
                    }
                    else
                    {
                        queryResponse(result.first, result.nrows, result.result);
                    }
                });
        }
    }

    protected void destroy()
    {
        if(_query != null)
        {
            _query.destroyAsync();
            _query = null;
        }
    }

    synchronized public void setListener(Listener cb)
    {
        _listener = cb;
        _listener.onDataChange(getQueryModel(), false);
        if(_lastError != null)
        {
            _listener.onError();
        }
    }

    synchronized public String getLastError()
    {
        return _lastError;
    }

    synchronized public void clearLastError()
    {
        _lastError = null;
    }

    synchronized public void getMore(int position)
    {
        if(com.zeroc.library.BuildConfig.DEBUG && position >= _nrows)
        {
            throw  new AssertionError();
        }
        if(position < _rowsQueried)
        {
            return;
        }

        _query.nextAsync(10).whenComplete((result, ex) ->
            {
                if(ex != null)
                {
                    postError(ex.toString());
                }
                else
                {
                    synchronized(this)
                    {
                        _books.addAll(result.returnValue);
                        postDataChanged(false);
                    }
                }
            });
        _rowsQueried += 10;
    }

    synchronized public boolean setCurrentBook(int row)
    {
        if(row < _books.size())
        {
            _currentBook = row;
            return true;
        }
        return false;
    }

    synchronized public void returnBook()
    {
        if(com.zeroc.library.BuildConfig.DEBUG && _currentBook == NO_BOOK)
        {
            throw  new AssertionError();
        }
        final BookDescription desc = _books.get(_currentBook);
        desc.proxy.returnBookAsync().whenComplete((result, ex) ->
            {
                if(ex != null)
                {
                    final String error;
                    if(ex instanceof BookNotRentedException)
                    {
                        error = "The book is no longer rented.";

                        desc.rentedBy = "";
                        postDataChanged(false);
                    }
                    else
                    {
                        error = "An error occurred: " + ex.toString();
                    }
                    postError(error);
                }
                else
                {
                    synchronized(this)
                    {
                        desc.rentedBy = "";
                        postDataChanged(false);
                    }
                }
            });
    }

    synchronized public void rentBook(final String r)
    {
        if(com.zeroc.library.BuildConfig.DEBUG && _currentBook == NO_BOOK)
        {
            throw  new AssertionError();
        }
        final BookDescription desc = _books.get(_currentBook);
        desc.proxy.rentBookAsync(r).whenComplete((result, ex) ->
            {
                if(ex != null)
                {
                    final String error;
                    if(ex instanceof InvalidCustomerException)
                    {
                        error = "The customer name is invalid.";
                    }
                    else if(ex instanceof BookRentedException)
                    {
                        error = "That book is already rented.";

                        BookRentedException bre = (BookRentedException)ex;
                        desc.rentedBy = bre.renter;
                        postDataChanged(false);
                    }
                    else
                    {
                        error = "An error occurred: " + ex.toString();
                    }
                    postError(error);
                }
                else
                {
                    synchronized(this)
                    {
                        desc.rentedBy = r;
                        postDataChanged(false);
                    }
                }
            });
    }

    synchronized public void deleteBook()
    {
        if(com.zeroc.library.BuildConfig.DEBUG && _currentBook == NO_BOOK)
        {
            throw  new AssertionError();
        }
        final BookDescription desc = _books.get(_currentBook);
        desc.proxy.destroyAsync().whenComplete((result, ex) ->
            {
                if(ex != null)
                {
                    postError(ex.toString());
                }
                else
                {
                    synchronized(this)
                    {
                        _books.remove(_currentBook);
                        _currentBook = NO_BOOK;
                        --_nrows;
                        postDataChanged(false);
                    }
                }
            });
    }

    synchronized public boolean saveBook(final BookDescription newDesc)
    {
        if(com.zeroc.library.BuildConfig.DEBUG && _currentBook == NO_BOOK)
        {
            throw  new AssertionError();
        }
        if(_currentBook == NEW_BOOK)
        {
            _library.createBookAsync(newDesc.isbn, newDesc.title, newDesc.authors).whenComplete((result, ex) ->
                {
                    if(ex != null)
                    {
                        if(ex instanceof BookExistsException)
                        {
                            postError("That ISBN is already in the library.");
                        }
                        else
                        {
                            postError("Unknown error: " + ex.toString());
                        }
                    }
                    else
                    {
                        synchronized(this)
                        {
                            _currentBook = NO_BOOK;
                            postDataChanged(true);
                        }
                    }
                });

            return true;
        }

        final BookDescription desc = _books.get(_currentBook);

        final boolean saveTitle = !newDesc.title.equals(desc.title);
        final boolean saveAuthors = !newDesc.authors.equals(desc.authors);

        // If nothing changed we're done.
        if(!saveTitle && !saveAuthors)
        {
            return false;
        }

        Runnable r = () ->
            {
                try
                {
                    if(saveTitle)
                    {
                        desc.proxy.setTitle(newDesc.title);
                        desc.title = newDesc.title;
                    }
                    if(saveAuthors)
                    {
                        desc.proxy.setAuthors(newDesc.authors);
                        desc.authors = newDesc.authors;
                    }
                    postDataChanged(true);
                }
                catch(com.zeroc.Ice.LocalException ex)
                {
                    postError(ex.toString());
                }
            };
        new Thread(r).start();
        return true;
    }
}
