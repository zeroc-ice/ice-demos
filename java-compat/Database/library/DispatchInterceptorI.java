// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

class DispatchInterceptorI extends Ice.DispatchInterceptor
{
    @Override
    public boolean
    dispatch(Ice.Request request)
        throws Ice.UserException
    {
        // Allocate a new SQLRequestContext associated with this
        // request thread.
        SQLRequestContext context = new SQLRequestContext();
        try
        {
            return _servant.ice_dispatch(request);
        }
        catch(JDBCException ex)
        {
            // Log the error.
            Ice.Current c = request.getCurrent();
            context.error("call of `" + c.operation + "' on id `" + c.id.category + "/" + c.id.name + "' failed", ex);

            // A JDBCException causes the current transaction to
            // rollback.
            context.destroyFromDispatch(false);

            // Translate the exception to UnknownException.
            Ice.UnknownException e = new Ice.UnknownException();
            e.initCause(ex);
            throw e;
        }
        catch(Ice.UserException ex)
        {
            // A user exception causes the transaction to rollback.
            context.destroyFromDispatch(false);
            throw ex;
        }
        catch(RuntimeException ex)
        {
            // Any other exception causes the transaction to rollback.
            context.destroyFromDispatch(false);
            throw ex;
        }
    }

    DispatchInterceptorI(Ice.Object servant)
    {
        _servant = servant;
    }

    private Ice.Object _servant;
}
