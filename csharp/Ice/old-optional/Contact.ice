// Copyright (c) ZeroC, Inc.

#pragma once

module Demo
{
    enum NumberType
    {
        HOME,
        OFFICE,
        CELL
    }

    class Contact
    {
        string name;
        optional(1) NumberType type = HOME;
        optional(2) string number;
        optional(3) int dialGroup;
    }

    interface ContactDB
    {
        void addContact(string name, optional(1) NumberType type, optional(2) string number, optional(3) int dialGroup);
        void updateContact(string name, optional(1) NumberType type, optional(2) string number, optional(3) int dialGroup);

        Contact query(string name);
        optional(1) string queryNumber(string name);
        void queryDialgroup(string name, out optional(1) int dialGroup);

        void shutdown();
    }
}
