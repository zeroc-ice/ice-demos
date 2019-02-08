//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.IceDiscovery.hello.Demo.*;

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
            communicator.getProperties().setProperty("Ice.Default.Package", "com.zeroc.demos.IceDiscovery.hello");
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

        HelloPrx twoway = HelloPrx.checkedCast(communicator.stringToProxy("hello"));
        if(twoway == null)
        {
            System.err.println("invalid proxy");
            return 1;
        }
        HelloPrx oneway = twoway.ice_oneway();
        HelloPrx batchOneway = twoway.ice_batchOneway();
        HelloPrx datagram = twoway.ice_datagram();
        HelloPrx batchDatagram = twoway.ice_batchDatagram();

        boolean secure = false;
        int timeout = -1;
        int delay = 0;

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
                    twoway.sayHello(delay);
                }
                else if(line.equals("o"))
                {
                    oneway.sayHello(delay);
                }
                else if(line.equals("O"))
                {
                    batchOneway.sayHello(delay);
                }
                else if(line.equals("d"))
                {
                    if(secure)
                    {
                        System.out.println("secure datagrams are not supported");
                    }
                    else
                    {
                        datagram.sayHello(delay);
                    }
                }
                else if(line.equals("D"))
                {
                    if(secure)
                    {
                        System.out.println("secure datagrams are not supported");
                    }
                    else
                    {
                        batchDatagram.sayHello(delay);
                    }
                }
                else if(line.equals("f"))
                {
                    batchOneway.ice_flushBatchRequests();
                    if(!secure)
                    {
                        batchDatagram.ice_flushBatchRequests();
                    }
                }
                else if(line.equals("T"))
                {
                    if(timeout == -1)
                    {
                        timeout = 2000;
                    }
                    else
                    {
                        timeout = -1;
                    }

                    twoway = twoway.ice_invocationTimeout(timeout);
                    oneway = oneway.ice_invocationTimeout(timeout);
                    batchOneway = batchOneway.ice_invocationTimeout(timeout);

                    if(timeout == -1)
                    {
                        System.out.println("timeout is now switched off");
                    }
                    else
                    {
                        System.out.println("timeout is now set to 2000ms");
                    }
                }
                else if(line.equals("P"))
                {
                    if(delay == 0)
                    {
                        delay = 2500;
                    }
                    else
                    {
                        delay = 0;
                    }

                    if(delay == 0)
                    {
                        System.out.println("server delay is now deactivated");
                    }
                    else
                    {
                        System.out.println("server delay is now set to 2500ms");
                    }
                }
                else if(line.equals("S"))
                {
                    secure = !secure;

                    twoway = twoway.ice_secure(secure);
                    oneway = oneway.ice_secure(secure);
                    batchOneway = batchOneway.ice_secure(secure);
                    datagram = datagram.ice_secure(secure);
                    batchDatagram = batchDatagram.ice_secure(secure);

                    if(secure)
                    {
                        System.out.println("secure mode is now on");
                    }
                    else
                    {
                        System.out.println("secure mode is now off");
                    }
                }
                else if(line.equals("s"))
                {
                    twoway.shutdown();
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
            "T: set a timeout\n" +
            "P: set a server delay\n" +
            "S: switch secure mode on/off\n" +
            "s: shutdown server\n" +
            "x: exit\n" +
            "?: help\n");
    }
}
