// Copyright (c) ZeroC, Inc.

#pragma once

module Filesystem
{
    exception GenericError
    {
        string reason;
    }

    interface Node
    {
        idempotent string name();
    }

    sequence<string> Lines;

    interface File extends Node
    {
        idempotent Lines read();
        idempotent void write(Lines text) throws GenericError;
    }

    sequence<Node*> NodeSeq;

    interface Directory extends Node
    {
        idempotent NodeSeq list();
    }
}
