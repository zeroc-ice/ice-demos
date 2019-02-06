//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Ice.minimal.Demo.*;

public class Client
{
    public static void main(String[] args)
    {
        try(com.zeroc.Ice.Communicator communicator = com.zeroc.Ice.Util.initialize(args))
        {
            communicator.getProperties().setProperty("Ice.Default.Package", "com.zeroc.demos.Ice.minimal");
            HelloPrx hello = HelloPrx.checkedCast(communicator.stringToProxy("hello:default -h localhost -p 10000"));
            hello.sayHello();
        }
    }
}
