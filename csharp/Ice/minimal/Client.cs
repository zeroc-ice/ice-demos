// Copyright (c) ZeroC, Inc.

using Demo;

internal class Client
{
    public static int Main(string[] args)
    {
        try
        {
            using var communicator = Ice.Util.initialize(ref args);
            var hello = HelloPrxHelper.checkedCast(
                communicator.stringToProxy("hello:default -h localhost -p 10000"));
            hello.sayHello();
            return 0;
        }
        catch (Exception ex)
        {
            Console.Error.WriteLine(ex);
            return 1;
        }
    }
}
