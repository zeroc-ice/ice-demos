// Copyright (c) ZeroC, Inc.

import com.zeroc.demos.Ice.minimal.Demo.*;

public class Printer implements Hello
{
    @Override
    public void sayHello(com.zeroc.Ice.Current current)
    {
        System.out.println("Hello World!");
    }
}
