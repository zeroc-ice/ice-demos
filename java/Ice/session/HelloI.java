//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Ice.session.Demo.*;

public class HelloI implements Hello
{
    public HelloI(String name, int id)
    {
        _name = name;
        _id = id;
    }

    @Override
    public void sayHello(com.zeroc.Ice.Current current)
    {
        System.out.println("Hello object #" + _id + " for session `" + _name + "' says:\n" + "Hello " + _name + "!");
    }

    final private String _name;
    final private int _id;
}
