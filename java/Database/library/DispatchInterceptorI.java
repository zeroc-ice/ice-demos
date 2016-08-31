// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

import java.util.concurrent.CompletionStage;

import com.zeroc.Ice.DispatchInterceptor;
import com.zeroc.Ice.OutputStream;
import com.zeroc.Ice.Request;
import com.zeroc.Ice.UserException;

class DispatchInterceptorI extends DispatchInterceptor
{
    @Override
    public CompletionStage<OutputStream> dispatch(Request request)
        throws UserException
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
            com.zeroc.Ice.Current c = request.getCurrent();
            context.error("call of `" + c.operation + "' on id `" + c.id.category + "/" + c.id.name + "' failed", ex);

            // A JDBCException causes the current transaction to
            // rollback.
            context.destroyFromDispatch(false);

            // Translate the exception to UnknownException.
            com.zeroc.Ice.UnknownException e = new com.zeroc.Ice.UnknownException();
            e.initCause(ex);
            throw e;
        }
        catch(UserException ex)
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

    DispatchInterceptorI(com.zeroc.Ice.Object servant)
    {
        _servant = servant;
    }

    private com.zeroc.Ice.Object _servant;
}
