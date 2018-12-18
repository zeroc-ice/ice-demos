// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

public class HelloI extends _HelloDisp
{
    @Override
    public void
    sayHello(Ice.Current current)
    {
        System.out.println("Hello World!");
    }
}
