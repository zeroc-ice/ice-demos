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

        Ice.ObjectAdapter adapter = communicator().createObjectAdapter("Hello");
        Ice.Properties properties = communicator().getProperties();
        Ice.Identity id = Ice.Util.stringToIdentity(properties.getProperty("Identity"));
        adapter.add(new HelloI(properties.getProperty("Ice.ProgramName")), id);
        adapter.activate();
        communicator().waitForShutdown();
        return 0;
    }

    static public void
    main(String[] args)
    {
        Server app = new Server();
        int status = app.main("Server", args);
        System.exit(status);
    }
}
