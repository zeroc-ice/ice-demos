//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Manual.simpleFilesystem.Filesystem.*;

final class FileI extends NodeI implements File
{
    // FileI constructor
    FileI(String name, DirectoryI parent)
    {
        super(name, parent);
    }

    // Slice File::read() operation
    @Override public String[] read(com.zeroc.Ice.Current current)
    {
        return _lines;
    }

    // Slice File::write() operation
    @Override public void write(String[] text, com.zeroc.Ice.Current current)
        throws GenericError
    {
        _lines = text;
    }

    private String[] _lines;
}
