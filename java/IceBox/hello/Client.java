//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.IceBox.hello.Demo.*;

public class Client
{
    public static void main(String[] args)
    {
        int status = 0;
        java.util.List<String> extraArgs = new java.util.ArrayList<>();

        //
        // Try with resources block - communicator is automatically destroyed
        // at the end of this try block
        //
        try(com.zeroc.Ice.Communicator communicator = com.zeroc.Ice.Util.initialize(args, "config.client", extraArgs))
        {
            communicator.getProperties().setProperty("Ice.Default.Package", "com.zeroc.demos.IceBox.hello");
            if(!extraArgs.isEmpty())
            {
                System.err.println("too many arguments");
                status = 1;
            }
            else
            {
                status = run(communicator);
            }
        }

        System.exit(status);
    }

    private static int run(com.zeroc.Ice.Communicator communicator)
    {

        HelloPrx twoway = HelloPrx.checkedCast(
            communicator.propertyToProxy("Hello.Proxy")).ice_twoway().ice_timeout(-1).ice_secure(false);
        if(twoway == null)
        {
            System.err.println("invalid object reference");
            return 1;
        }
        HelloPrx oneway = twoway.ice_oneway();
        HelloPrx batchOneway = twoway.ice_batchOneway();
        HelloPrx datagram = twoway.ice_datagram();
        HelloPrx batchDatagram = twoway.ice_batchDatagram();

        menu();

        java.io.BufferedReader in = new java.io.BufferedReader(new java.io.InputStreamReader(System.in));

        String line = null;
        do
        {
            try
            {
                System.out.print("==> ");
                System.out.flush();
                line = in.readLine();
                if(line == null)
                {
                    break;
                }
                if(line.equals("t"))
                {
                    twoway.sayHello();
                }
                else if(line.equals("o"))
                {
                    oneway.sayHello();
                }
                else if(line.equals("O"))
                {
                    batchOneway.sayHello();
                }
                else if(line.equals("d"))
                {
                    datagram.sayHello();
                }
                else if(line.equals("D"))
                {
                    batchDatagram.sayHello();
                }
                else if(line.equals("f"))
                {
                    batchOneway.ice_flushBatchRequests();
                    batchDatagram.ice_flushBatchRequests();
                }
                else if(line.equals("x"))
                {
                    // Nothing to do
                }
                else if(line.equals("?"))
                {
                    menu();
                }
                else
                {
                    System.out.println("unknown command `" + line + "'");
                    menu();
                }
            }
            catch(java.io.IOException ex)
            {
                ex.printStackTrace();
            }
            catch(com.zeroc.Ice.LocalException ex)
            {
                ex.printStackTrace();
            }
        }
        while(!line.equals("x"));

        return 0;
    }

    private static void menu()
    {
        System.out.println(
            "usage:\n" +
            "t: send greeting as twoway\n" +
            "o: send greeting as oneway\n" +
            "O: send greeting as batch oneway\n" +
            "d: send greeting as datagram\n" +
            "D: send greeting as batch datagram\n" +
            "f: flush all batch requests\n" +
            "x: exit\n" +
            "?: help\n");
    }
}
