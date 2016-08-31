// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

public class Client extends Ice.Application
{
    class ShutdownHook extends Thread
    {
        @Override
        public void
        run()
        {
            try
            {
                communicator().destroy();
            }
            catch(Ice.LocalException ex)
            {
                ex.printStackTrace();
            }
        }
    }

    @Override
    public int
    run(String[] args)
    {
        if(args.length > 1)
        {
            System.err.println("Usage: " + appName() + " [file]");
            return 1;
        }

        //
        // Since this is an interactive demo we want to clear the
        // Application installed interrupt callback and install our
        // own shutdown hook.
        //
        setInterruptHook(new ShutdownHook());

        return RunParser.runParser(appName(), args, communicator());
    }

    static public void
    main(String[] args)
    {
        Client app = new Client();
        int status = app.main("demo.Database.library.Client", args, "config.client");
        System.exit(status);
    }
}
