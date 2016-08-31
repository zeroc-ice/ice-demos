// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Demo.*;

import java.util.Optional;
import java.util.OptionalInt;

public class Client extends com.zeroc.Ice.Application
{
    class ShutdownHook extends Thread
    {
        @Override
        public void run()
        {
            try
            {
                communicator().destroy();
            }
            catch(com.zeroc.Ice.LocalException ex)
            {
                ex.printStackTrace();
            }
        }
    }

    @Override
    public int run(String[] args)
    {
        if(args.length > 0)
        {
            System.err.println(appName() + ": too many arguments");
            return 1;
        }

        ContactDBPrx contactdb = ContactDBPrx.checkedCast(communicator().propertyToProxy("ContactDB.Proxy"));
        if(contactdb == null)
        {
            System.err.println(appName() + ": invalid proxy");
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
        Optional<String> number = contactdb.queryNumber("john");

        //
        // isSet() tests if an optional value is set.
        //
        if(!number.isPresent())
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
        OptionalInt dialgroup = contactdb.queryDialgroup("john");
        if(!dialgroup.isPresent() || dialgroup.getAsInt() != 0)
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
        contactdb.addContact("steve", null, Optional.of(steveNumber), OptionalInt.of(1));

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

        dialgroup = contactdb.queryDialgroup("steve");
        if(!dialgroup.isPresent() || dialgroup.getAsInt() != 1)
        {
            System.out.print("dialgroup is incorrect ");
        }

        System.out.println("ok");

        //
        // Add a contact from "frank". Here the dialGroup field isn't set.
        //
        String frankNumber = "345-678-9012";
        contactdb.addContact("frank", Optional.of(NumberType.CELL), Optional.of(frankNumber), null);

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

        dialgroup = contactdb.queryDialgroup("frank");
        if(dialgroup.isPresent())
        {
            System.out.print("dialgroup is incorrect ");
        }
        System.out.println("ok");

        //
        // Add a contact from "anne". The number field isn't set.
        //
        contactdb.addContact("anne", Optional.of(NumberType.OFFICE), null, OptionalInt.of(2));

        System.out.print("Checking anne... ");
        number = contactdb.queryNumber("anne");
        if(number.isPresent())
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

        dialgroup = contactdb.queryDialgroup("anne");
        if(!dialgroup.isPresent() || dialgroup.getAsInt() != 2)
        {
            System.out.print("dialgroup is incorrect ");
        }

        //
        // The optional fields can be used to determine what fields to
        // update on the contact.  Here we update only the number for anne,
        // the remainder of the fields are unchanged.
        //
        String anneNumber = "456-789-0123";
        contactdb.updateContact("anne", null, Optional.of(anneNumber), null);
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

    public static void
    main(String[] args)
    {
        Client app = new Client();
        int status = app.main("Client", args, "config.client");
        System.exit(status);
    }
}

