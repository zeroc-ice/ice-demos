// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

public class Client
{
    public static void main(String[] args)
    {
        int status = 0;
        Ice.StringSeqHolder argsHolder = new Ice.StringSeqHolder(args);

        //
        // Try with resources block - communicator is automatically destroyed
        // at the end of this try block
        //
        try(Ice.Communicator communicator = Ice.Util.initialize(argsHolder, "config.client"))
        {
            if(argsHolder.value.length > 0)
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

    private static int run(Ice.Communicator communicator)
    {
        PropsPrx props = PropsPrxHelper.checkedCast(communicator.propertyToProxy("Props.Proxy"));
        if(props == null)
        {
            System.err.println("invalid proxy");
            return 1;
        }

        Ice.PropertiesAdminPrx admin =
            Ice.PropertiesAdminPrxHelper.checkedCast(communicator.propertyToProxy("Admin.Proxy"));

        java.util.List<String> keys = java.util.Arrays.asList("Demo.Prop1", "Demo.Prop2", "Demo.Prop3");

        java.util.Map<String, String> batch1 = new java.util.HashMap<String, String>();
        batch1.put("Demo.Prop1", "1");
        batch1.put("Demo.Prop2", "2");
        batch1.put("Demo.Prop3", "3");

        java.util.Map<String, String> batch2 = new java.util.HashMap<String, String>();
        batch2.put("Demo.Prop1", "10");
        batch2.put("Demo.Prop2", ""); // An empty value removes this property
        batch2.put("Demo.Prop3", "30");

        show(admin);
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
                if(line.equals("1") || line.equals("2"))
                {
                    java.util.Map<String, String> dict = line.equals("1") ? batch1 : batch2;
                    System.out.println("Sending:");
                    for(String key : keys)
                    {
                        String value = dict.get(key);
                        if(value != null)
                        {
                            System.out.println("  " + key + "=" + value);
                        }
                    }
                    System.out.println();

                    admin.setProperties(dict);

                    System.out.println("Changes:");
                    java.util.Map<String, String> changes = props.getChanges();
                    if(changes.isEmpty())
                    {
                        System.out.println("  None.");
                    }
                    else
                    {
                        for(String key : keys)
                        {
                            System.out.print("  " + key);
                            String value = dict.get(key);
                            if(value == null || value.length() == 0)
                            {
                                System.out.println(" was removed");
                            }
                            else
                            {
                                System.out.println(" is now " + value);
                            }
                        }
                    }
                }
                else if(line.equals("c"))
                {
                    show(admin);
                }
                else if(line.equals("s"))
                {
                    props.shutdown();
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
            catch(Ice.LocalException ex)
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
            "\n" +
            "usage:\n" +
            "1: set properties (batch 1)\n" +
            "2: set properties (batch 2)\n" +
            "c: show current properties\n" +
            "s: shutdown server\n" +
            "x: exit\n" +
            "?: help\n");
    }

    private static void show(Ice.PropertiesAdminPrx admin)
    {
        java.util.Map<String, String> props = admin.getPropertiesForPrefix("Demo");
        System.out.println("Server's current settings:");
        for(java.util.Map.Entry<String, String> e : props.entrySet())
        {
            System.out.println("  " + e.getKey() + "=" + e.getValue());
        }
    }
}
