// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;
import Ice.Optional;

public class Client
{
    static class ShutdownHook extends Thread
    {
        @Override
        public void
        run()
        {
            _communicator.destroy();
        }

        ShutdownHook(Ice.Communicator communicator)
        {
            _communicator = communicator;
        }

        private final Ice.Communicator _communicator;
    }

    public static void
    main(String[] args)
    {
        int status = 0;
        Ice.StringSeqHolder argsHolder = new Ice.StringSeqHolder(args);

        //
        // Try with resources block - communicator is automatically destroyed
        // at the end of this try block
        //
        try(Ice.Communicator communicator = Ice.Util.initialize(argsHolder, "config.client"))
        {
            //
            // Install shutdown hook to (also) destroy communicator during JVM shutdown.
            // This ensures the communicator gets destroyed when the user interrupts the application with Ctrl-C.
            //
            Runtime.getRuntime().addShutdownHook(new ShutdownHook(communicator));

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

    private static int
    run(Ice.Communicator communicator)
    {
        ContactDBPrx contactdb = ContactDBPrxHelper.checkedCast(communicator.propertyToProxy("ContactDB.Proxy"));
        if(contactdb == null)
        {
            System.err.println("invalid proxy");
            return 1;
        }

        //
        // Add a contact for "john". In this case since all parameters are provided it isn't
        // necessary to construct an Optional.
        //
        String johnNumber = "123-456-7890";
        contactdb.addContact("john", NumberType.HOME, johnNumber, 0);

        System.out.print("Checking john... ");

        //
        // Find the phone number for "john". Note the use of the Ice.Optional
        // for non-generic types.
        //
        Ice.Optional<String> number = contactdb.queryNumber("john");

        //
        // isSet() tests if an optional value is set.
        //
        if(!number.isSet())
        {
            System.out.print("number is incorrect ");
        }

        //
        // Call get() to retrieve the optional value.
        //
        if(!number.get().equals(johnNumber))
        {
            System.out.print("number is incorrect ");
        }

        // Optional can also be used in an out parameter. Note that
        // primitive types don't use Ice.Optional.
        Ice.IntOptional dialgroup = new Ice.IntOptional();
        contactdb.queryDialgroup("john", dialgroup);
        if(!dialgroup.isSet() || dialgroup.get() != 0)
        {
            System.out.print("dialgroup is incorrect ");
        }

        Contact info = contactdb.query("john");

        //
        // All of the info parameters should be set. On a class call
        // has<name> to find out whether the value is set.
        //
        if(!info.hasType() || !info.hasNumber() || !info.hasDialGroup())
        {
            System.out.print("info is incorrect ");
        }
        // Call get<name> to retrieve the value.
        if(info.getType() != NumberType.HOME || !info.getNumber().equals(johnNumber) || info.getDialGroup() != 0)
        {
            System.out.print("info is incorrect ");
        }
        System.out.println("ok");

        //
        // Add a contact for "steve". Here we have to construct Optional parameters
        // since we are not passing the NumberType parameter.
        //
        // The behavior of the server is to default construct the
        // Contact, and then assign  all set parameters.  Since the
        // default value of NumberType in the slice definition is
        // NumberType.HOME and in this case the NumberType is unset
        // it will take the default value.
        //
        // The java mapping permits null to be passed to unset optional values.
        //
        String steveNumber = "234-567-8901";
        contactdb.addContact("steve", null, Optional.O(steveNumber), Optional.O(1));

        System.out.print("Checking steve... ");
        number = contactdb.queryNumber("steve");
        if(!number.get().equals(steveNumber))
        {
            System.out.print("number is incorrect ");
        }

        info = contactdb.query("steve");
        //
        // Check the value for the NumberType.
        //
        if(!info.hasType() || info.getType() != NumberType.HOME)
        {
            System.out.print("info is incorrect ");
        }

        if(!info.getNumber().equals(steveNumber) || info.getDialGroup() != 1)
        {
            System.out.print("info is incorrect ");
        }

        contactdb.queryDialgroup("steve", dialgroup);
        if(!dialgroup.isSet() || dialgroup.get() != 1)
        {
            System.out.print("dialgroup is incorrect ");
        }

        System.out.println("ok");

        //
        // Add a contact from "frank". Here the dialGroup field isn't set.
        //
        String frankNumber = "345-678-9012";
        contactdb.addContact("frank", Optional.O(NumberType.CELL), Optional.O(frankNumber), null);

        System.out.print("Checking frank... ");

        number = contactdb.queryNumber("frank");
        if(!number.get().equals(frankNumber))
        {
            System.out.print("number is incorrect ");
        }

        info = contactdb.query("frank");
        //
        // The dial group field should be unset.
        //
        if(info.hasDialGroup())
        {
            System.out.print("info is incorrect ");
        }
        if(info.getType() != NumberType.CELL || !info.getNumber().equals(frankNumber))
        {
            System.out.print("info is incorrect ");
        }

        contactdb.queryDialgroup("frank", dialgroup);
        if(dialgroup.isSet())
        {
            System.out.print("dialgroup is incorrect ");
        }
        System.out.println("ok");

        //
        // Add a contact from "anne". The number field isn't set.
        //
        contactdb.addContact("anne", Optional.O(NumberType.OFFICE), null, Optional.O(2));

        System.out.print("Checking anne... ");
        number = contactdb.queryNumber("anne");
        if(number.isSet())
        {
            System.out.print("number is incorrect ");
        }

        info = contactdb.query("anne");
        //
        // The number field should be unset.
        //
        if(info.hasNumber())
        {
            System.out.print("info is incorrect ");
        }
        if(info.getType() != NumberType.OFFICE || info.getDialGroup() != 2)
        {
            System.out.print("info is incorrect ");
        }

        contactdb.queryDialgroup("anne", dialgroup);
        if(!dialgroup.isSet() || dialgroup.get() != 2)
        {
            System.out.print("dialgroup is incorrect ");
        }

        //
        // The optional fields can be used to determine what fields to
        // update on the contact.  Here we update only the number for anne,
        // the remainder of the fields are unchanged.
        //
        String anneNumber = "456-789-0123";
        contactdb.updateContact("anne", null, Optional.O(anneNumber), null);
        number = contactdb.queryNumber("anne");
        if(!number.get().equals(anneNumber))
        {
            System.out.print("number is incorrect ");
        }
        info = contactdb.query("anne");
        if(!info.getNumber().equals(anneNumber) || info.getType() != NumberType.OFFICE || info.getDialGroup() != 2)
        {
            System.out.print("info is incorrect ");
        }
        System.out.println("ok");

        contactdb.shutdown();

        return 0;
    }
}
