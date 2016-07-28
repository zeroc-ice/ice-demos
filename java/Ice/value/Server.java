// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

public class Server extends Ice.Application
{
    @Override
    public int
    run(String[] args)
    {
        if(args.length > 0)
        {
            System.err.println(appName() + ": too many arguments");
            return 1;
        }

        Ice.ValueFactory factory = new ValueFactory();
        communicator().getValueFactoryManager().add(factory, Demo.Printer.ice_staticId());

        Ice.ObjectAdapter adapter = communicator().createObjectAdapter("Value");
        Ice.Object object = new InitialI(adapter);
        adapter.add(object, Ice.Util.stringToIdentity("initial"));
        adapter.activate();
        communicator().waitForShutdown();
        return 0;
    }

    public static void
    main(String[] args)
    {
        Server app = new Server();
        int status = app.main("Server", args, "config.server");
        System.exit(status);
    }
}
