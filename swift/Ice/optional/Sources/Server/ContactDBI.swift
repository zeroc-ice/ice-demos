//
// Copyright (c) ZeroC, Inc. All rights reserved.
//
import Ice

class ContactDBI: ContactDB {
    var contacts = [String: Contact]()

    func addContact(name: String, type: NumberType?, number: String?, dialGroup: Int32?, current _: Ice.Current) {
        let contact = Contact()
        contact.name = name

        if let type = type {
            contact.type = type
        }

        if let number = number {
            contact.number = number
        }

        if let dialGroup = dialGroup {
            contact.dialGroup = dialGroup
        }
        contacts[name] = contact
    }

    func updateContact(name: String, type: NumberType?, number: String?, dialGroup: Int32?, current _: Ice.Current) {
        if let c = contacts[name] {
            if let type = type {
                c.type = type
            }

            if let number = number {
                c.number = number
            }

            if let dialGroup = dialGroup {
                c.dialGroup = dialGroup
            }
        }
    }

    func query(name: String, current _: Ice.Current) -> Contact? {
        return contacts[name]
    }

    func queryNumber(name: String, current _: Ice.Current) -> String? {
        return contacts[name]?.number
    }

    func queryDialgroup(name: String, current _: Ice.Current) -> Int32? {
        return contacts[name]?.dialGroup
    }

    func shutdown(current: Ice.Current) {
        print("Shutting down...")
        current.adapter!.getCommunicator().shutdown()
    }
}
