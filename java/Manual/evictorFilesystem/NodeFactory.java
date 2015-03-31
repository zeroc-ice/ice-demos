// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

import Filesystem.*;

public class NodeFactory implements Ice.ObjectFactory
{
    @Override
    public Ice.Object
    create(String type)
    {
        if(type.equals(PersistentFile.ice_staticId()))
        {
            return new FileI();
        }
        else if(type.equals(PersistentDirectory.ice_staticId()))
        {
            return new DirectoryI();
        }
        else
        {
            assert(false);
            return null;
        }
    }

    @Override
    public void
    destroy()
    {
    }
}
