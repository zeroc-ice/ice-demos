// Copyright (c) ZeroC, Inc.

using Demo;

internal class HelloI(string name, int id) : HelloDisp_
{
    public override void sayHello(Ice.Current c)
    {
        Console.Out.WriteLine("Hello object #" + _id + " for session `" + _name + "' says:\n" +
                              "Hello " + _name + "!");
    }

    private string _name = name;
    private readonly int _id = id;
}
