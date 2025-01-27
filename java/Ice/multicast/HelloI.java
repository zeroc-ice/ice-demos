// Copyright (c) ZeroC, Inc.

import com.zeroc.demos.Ice.multicast.Demo.*;

public class HelloI implements Hello
{
    @Override
    public void sayHello(com.zeroc.Ice.Current current)
    {
        System.out.println("Hello World!");
    }
}
