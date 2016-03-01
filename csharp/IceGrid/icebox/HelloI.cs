// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Demo;

public class HelloI : HelloDisp_
{
    public HelloI(string serviceName)
    {
        _serviceName = serviceName;
    }

    public override void sayHello(Ice.Current current)
    {
        string lang = System.Environment.GetEnvironmentVariable("LANG") != null ?
                                                            System.Environment.GetEnvironmentVariable("LANG") : "en";
        string greeting = "Hello, ";
        if(lang.Equals("fr"))
        {
            greeting = "Bonjour, ";
        }
        else if(lang.Equals("de"))
        {
            greeting = "Hallo, ";
        }
        else if(lang.Equals("es"))
        {
            greeting = "Hola, ";
        }
        else if(lang.Equals("it"))
        {
            greeting = "Ciao, ";
        }
        System.Console.Out.WriteLine(greeting + _serviceName);
    }

    private string _serviceName;
}
