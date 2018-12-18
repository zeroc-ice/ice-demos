// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

[["js:es6-module"]]

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
