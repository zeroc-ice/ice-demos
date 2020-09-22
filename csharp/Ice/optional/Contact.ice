//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

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
        tag(1) NumberType? type = HOME;
        tag(2) string? number;
        tag(3) int? dialGroup;
    }

    interface ContactDB
    {
        void addContact(string name, tag(1) NumberType? type, tag(2) string? number, tag(3) int? dialGroup);
        void updateContact(string name, tag(1) NumberType? type, tag(2) string? number, tag(3) int? dialGroup);

        Contact? query(string name);
        tag(1) string? queryNumber(string name);
        void queryDialgroup(string name, out tag(1) int? dialGroup);

        void shutdown();
    }
}
