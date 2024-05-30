//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

using Demo;
using System;

#nullable enable

public class Client
{
    public static int Main(string[] args)
    {
        int status = 0;

        try
        {
            //
            // using statement - communicator is automatically destroyed
            // at the end of this statement
            //
            using(var communicator = Ice.Util.initialize(ref args, "config.client"))
            {
                //
                // Destroy the communicator on Ctrl+C or Ctrl+Break
                //
                Console.CancelKeyPress += (sender, eventArgs) => communicator.destroy();

                if(args.Length > 0)
                {
                    Console.Error.WriteLine("too many arguments");
                    status = 1;
                }
                else
                {
                    status = run(communicator);
                }
            }
        }
        catch(Exception ex)
        {
            Console.Error.WriteLine(ex);
            status = 1;
        }

        return status;
    }

    private static int run(Ice.Communicator communicator)
    {
        var contactdb = ContactDBPrxHelper.uncheckedCast(communicator.propertyToProxy("ContactDB.Proxy"));
        if(contactdb == null)
        {
            Console.Error.WriteLine("invalid proxy");
            return 1;
        }

        //
        // Add a contact for "john". All parameters are provided.
        //
        var johnNumber = "123-456-7890";
        contactdb.addContact("john", NumberType.HOME, johnNumber, 0);

        Console.Write("Checking john... ");

        //
        // Find the phone number for "john".
        //
        var number = contactdb.queryNumber("john");

        //
        // HasValue tests if an optional value is set.
        //
        if(number is null)
        {
            Console.Write("number is incorrect ");
        }

        //
        // Call Value to retrieve the optional value.
        //
        if(number != johnNumber)
        {
            Console.Write("number is incorrect ");
        }

        // Optional can also be used in an out parameter.
        int? dialgroup;
        contactdb.queryDialgroup("john", out dialgroup);
        if(dialgroup != 0)
        {
            Console.Write("dialgroup is incorrect ");
        }

        Contact? info = contactdb.query("john");

        //
        // All of the info parameters should be set. Each of the optional members
        // of the class map to C# nullable type.
        //
        if(info?.type != NumberType.HOME || info.number != johnNumber || info.dialGroup != 0)
        {
            Console.Write("info is incorrect ");
        }
        Console.WriteLine("ok");

        //
        // Add a contact for "steve". The behavior of the server is to
        // default construct the Contact, and then assign all set parameters.
        // Since the default value of NumberType in the slice definition
        // is NumberType.HOME and in this case the NumberType is unset it will take
        // the default value.
        //
        var steveNumber = "234-567-8901";
        contactdb.addContact("steve", null, steveNumber, 1);

        Console.Write("Checking steve... ");
        number = contactdb.queryNumber("steve");
        if(number != steveNumber)
        {
            Console.Write("number is incorrect ");
        }

        info = contactdb.query("steve");
        //
        // Check the value for the NumberType.
        //
        if(info?.type != NumberType.HOME)
        {
            Console.Write("info is incorrect ");
        }

        if(info?.number != steveNumber || info.dialGroup != 1)
        {
            Console.Write("info is incorrect ");
        }

        contactdb.queryDialgroup("steve", out dialgroup);
        if(dialgroup != 1)
        {
            Console.Write("dialgroup is incorrect ");
        }

        Console.WriteLine("ok");

        //
        // Add a contact from "frank". Here the dialGroup field isn't set.
        //
        var frankNumber = "345-678-9012";
        contactdb.addContact("frank", NumberType.CELL, frankNumber, null);

        Console.Write("Checking frank... ");

        number = contactdb.queryNumber("frank");
        if(number != frankNumber)
        {
            Console.Write("number is incorrect ");
        }

        info = contactdb.query("frank");
        //
        // The dial group field should be unset.
        //
        if(info?.dialGroup is not null)
        {
            Console.Write("info is incorrect ");
        }
        if(info?.type != NumberType.CELL || info.number != frankNumber)
        {
            Console.Write("info is incorrect ");
        }

        contactdb.queryDialgroup("frank", out dialgroup);
        if(dialgroup is not null)
        {
            Console.Write("dialgroup is incorrect ");
        }
        Console.WriteLine("ok");

        //
        // Add a contact from "anne". The number field isn't set.
        //
        contactdb.addContact("anne", NumberType.OFFICE, null, 2);

        Console.Write("Checking anne... ");
        number = contactdb.queryNumber("anne");
        if(number is not null)
        {
            Console.Write("number is incorrect ");
        }

        info = contactdb.query("anne");
        //
        // The number field should be unset.
        //
        if(info?.number is not null)
        {
            Console.Write("info is incorrect ");
        }
        if(info?.type != NumberType.OFFICE || info.dialGroup != 2)
        {
            Console.Write("info is incorrect ");
        }

        contactdb.queryDialgroup("anne", out dialgroup);
        if(dialgroup != 2)
        {
            Console.Write("dialgroup is incorrect ");
        }

        //
        // The optional fields can be used to determine what fields to
        // update on the contact.  Here we update only the number for anne,
        // the remainder of the fields are unchanged.
        //
        var anneNumber = "456-789-0123";
        contactdb.updateContact("anne", null, anneNumber, null);
        number = contactdb.queryNumber("anne");
        if(number != anneNumber)
        {
            Console.Write("number is incorrect ");
        }
        info = contactdb.query("anne");
        if(info?.number != anneNumber || info.type != NumberType.OFFICE || info.dialGroup != 2)
        {
            Console.Write("info is incorrect ");
        }
        Console.WriteLine("ok");

        contactdb.shutdown();

        return 0;
    }
}
