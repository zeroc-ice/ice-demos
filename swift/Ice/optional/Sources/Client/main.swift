//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import Foundation
import Ice

func run() -> Int32 {
    do {
        var args = CommandLine.arguments
        let communicator = try Ice.initialize(args: &args, configFile: "config.client")
        defer {
            communicator.destroy()
        }

        guard args.count == 1 else {
            print("too many arguments")
            return 1
        }

        guard let base = try communicator.propertyToProxy("ContactDB.Proxy"),
            let contactdb = try checkedCast(prx: base, type: ContactDBPrx.self) else {
            print("invalid proxy")
            return 1
        }

        //
        // Add a contact for "john". All parameters are provided.
        //
        do {
            let johnNumber = "123-456-7890"
            try contactdb.addContact(name: "john", type: .HOME, number: johnNumber, dialGroup: 0)

            print("Checking john... ", terminator: "")

            //
            // Find the phone number for "john".
            //
            guard try contactdb.queryNumber("john") == johnNumber else {
                print("number is incorrect")
                return 1
            }

            //
            // Optional can also be used in an out parameter.
            //
            guard try contactdb.queryDialgroup("john") == 0 else {
                print("dialgroup is incorrect")
                return 1
            }

            guard let info = try contactdb.query("john"),
                info.type == NumberType.HOME,
                info.number == johnNumber,
                info.dialGroup == 0 else {
                print("info is incorrect")
                return 1
            }
        }
        print("ok")

        do {
            //
            // Add a contact for "steve". The behavior of the server is to
            // default construct the Contact, and then assign  all set parameters.
            // Since the default value of NumberType in the slice definition
            // is NumberType.HOME and in this case the NumberType is unset it will take
            // the default value.
            //
            // The Swift mapping permits to omit unset optional values, nil is also acepted
            // for unset optional values
            //
            let steveNumber = "234-567-8901"
            try contactdb.addContact(name: "steve", number: steveNumber, dialGroup: 1)

            print("Checking steve... ", terminator: "")
            guard try contactdb.queryNumber("steve") == steveNumber else {
                print("number is incorrect ")
                return 1
            }

            guard let info = try contactdb.query("steve"),
                info.type == .HOME,
                info.number == steveNumber,
                info.dialGroup == 1 else {
                print("info is incorrect")
                return 1
            }

            guard try contactdb.queryDialgroup("steve") == 1 else {
                print("dialgroup is incorrect")
                return 1
            }
        }
        print("ok")

        print("Checking frank... ", terminator: "")

        do {
            //
            // Add a contact from "frank". Here the dialGroup field isn't set.
            //
            let frankNumber = "345-678-9012"
            try contactdb.addContact(name: "frank", type: .CELL, number: frankNumber)

            guard try contactdb.queryNumber("frank") == frankNumber else {
                print("number is incorrect ")
                return 1
            }

            //
            // The dial group field should be unset.
            //
            guard let info = try contactdb.query("frank"),
                info.dialGroup == nil,
                info.type == .CELL,
                info.number == frankNumber else {
                print("info is incorrect")
                return 1
            }

            guard try contactdb.queryDialgroup("frank") == nil else {
                print("dialgroup is incorrect")
                return 1
            }
        }
        print("ok")

        print("Checking anne... ", terminator: "")

        do {
            //
            // Add a contact from "anne". The number field isn't set.
            //
            try contactdb.addContact(name: "anne", type: .OFFICE, dialGroup: 2)

            guard try contactdb.queryNumber("anne") == nil else {
                print("number is incorrect ")
                return 1
            }

            //
            // The number field should be unset.
            //
            guard let info = try contactdb.query("anne"),
                info.number == nil,
                info.type == .OFFICE,
                info.dialGroup == 2 else {
                print("info is incorrect ")
                return 1
            }

            guard try contactdb.queryDialgroup("anne") == 2 else {
                print("dialgroup is incorrect ")
                return 1
            }
        }

        do {
            //
            // The optional fields can be used to determine what fields to
            // update on the contact.  Here we update only the number for anne,
            // the remainder of the fields are unchanged.
            //
            let anneNumber = "456-789-0123"
            try contactdb.updateContact(name: "anne", number: anneNumber)
            guard try contactdb.queryNumber("anne") == anneNumber else {
                print("number is incorrect ")
                return 1
            }
            guard let info = try contactdb.query("anne"),
                info.number == anneNumber,
                info.type == .OFFICE,
                info.dialGroup == 2 else {
                print("info is incorrect")
                return 1
            }
        }
        print("ok")

        try contactdb.shutdown()

        return 0
    } catch {
        print("Error: \(error)\n")
        return 1
    }
}

exit(run())
