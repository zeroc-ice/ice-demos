// Copyright (c) ZeroC, Inc. All rights reserved.

using Demo;
using System;
using System.Configuration;
using System.Diagnostics;
using ZeroC.Ice;

// using statement - communicator is automatically destroyed at the end of this statement
using var communicator = new Communicator(ref args, ConfigurationManager.AppSettings);

// Destroy the communicator on Ctrl+C or Ctrl+Break
Console.CancelKeyPress += (sender, eventArgs) => communicator.DisposeAsync();

if (args.Length > 0)
{
    throw new ArgumentException("too many arguments");
}

IContactDBPrx? contactdb = communicator.GetPropertyAsProxy("ContactDB.Proxy", IContactDBPrx.Factory) ??
    throw new ArgumentException("invalid proxy");

// Add a contact for "john". All parameters are provided.
string johnNumber = "123-456-7890";
contactdb.AddContact("john", NumberType.HOME, johnNumber, 0);

Console.Write("Checking john... ");

// Find the phone number for "john".
string? number = contactdb.QueryNumber("john");

// HasValue tests if an optional value is set.
if (number != johnNumber)
{
    Console.Write("number is incorrect ");
}

// Optional can also be used in an out parameter.
int? dialgroup = contactdb.QueryDialgroup("john");
if (!dialgroup.HasValue || dialgroup.Value != 0)
{
    Console.Write("dialgroup is incorrect ");
}

Contact? info = contactdb.Query("john");

Debug.Assert(info != null);

// All of the info parameters should be set. Each of the optional members of the class map to Ice.Optional<T> member.
if (!info.Type.HasValue || !info.DialGroup.HasValue)
{
    Console.Write("info is incorrect ");
}
else if (info.Type.Value != NumberType.HOME || info.Number != johnNumber || info.DialGroup.Value != 0)
{
    Console.Write("info is incorrect ");
}
Console.WriteLine("ok");

// Add a contact for "steve".

string steveNumber = "234-567-8901";
contactdb.AddContact("steve", null, steveNumber, 1);

Console.Write("Checking steve... ");
number = contactdb.QueryNumber("steve");
if (number != steveNumber)
{
    Console.Write("number is incorrect ");
}

info = contactdb.Query("steve");
Debug.Assert(info != null);
// Check the value for the NumberType.
if (info.Type.HasValue)
{
    Console.Write("info is incorrect ");
}

if (info.Number != steveNumber || info.DialGroup != 1)
{
    Console.Write("info is incorrect ");
}

dialgroup = contactdb.QueryDialgroup("steve");
if (!dialgroup.HasValue || dialgroup.Value != 1)
{
    Console.Write("dialgroup is incorrect ");
}

Console.WriteLine("ok");

// Add a contact from "frank". Here the dialGroup field isn't set.
string frankNumber = "345-678-9012";
contactdb.AddContact("frank", NumberType.CELL, frankNumber, null);

Console.Write("Checking frank... ");

number = contactdb.QueryNumber("frank");
if (number != frankNumber)
{
    Console.Write("number is incorrect ");
}

info = contactdb.Query("frank");
Debug.Assert(info != null);

// The dial group field should be unset.
if (info.DialGroup.HasValue)
{
    Console.Write("info is incorrect ");
}

if (info.Type != NumberType.CELL || info.Number != frankNumber)
{
    Console.Write("info is incorrect ");
}

dialgroup = contactdb.QueryDialgroup("frank");
if (dialgroup.HasValue)
{
    Console.Write("dialgroup is incorrect ");
}
Console.WriteLine("ok");

// Add a contact from "anne". The number field isn't set.
contactdb.AddContact("anne", NumberType.OFFICE, null, 2);

Console.Write("Checking anne... ");
number = contactdb.QueryNumber("anne");
if (number != null)
{
    Console.Write("number is incorrect ");
}

info = contactdb.Query("anne");
Debug.Assert(info != null);

// The number field should be unset.
if (info.Number != null)
{
    Console.Write("info is incorrect ");
}

if (info.Type != NumberType.OFFICE || info.DialGroup != 2)
{
    Console.Write("info is incorrect ");
}

dialgroup = contactdb.QueryDialgroup("anne");
if (!dialgroup.HasValue || dialgroup.Value != 2)
{
    Console.Write("dialgroup is incorrect ");
}

// The optional fields can be used to determine what fields to update on the contact. Here we update only the number
// for anne, the remainder of the fields are unchanged.
string anneNumber = "456-789-0123";
contactdb.UpdateContact("anne", null, anneNumber, null);
number = contactdb.QueryNumber("anne");
if (number != anneNumber)
{
    Console.Write("number is incorrect ");
}

info = contactdb.Query("anne");
Debug.Assert(info != null);

if (info.Number != anneNumber || info.Type != NumberType.OFFICE || info.DialGroup != 2)
{
    Console.Write("info is incorrect ");
}
Console.WriteLine("ok");

contactdb.Shutdown();
