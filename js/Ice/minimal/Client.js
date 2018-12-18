// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

const Ice = require("ice").Ice;
const Demo = require("./generated/Hello").Demo;

(async () =>
{
    let communicator;
    try
    {
        communicator = Ice.initialize(process.argv);
        //
        // Down-cast the proxy to the hello object interface and invoke
        // the sayHello method.
        //
        const hello = await Demo.HelloPrx.checkedCast(communicator.stringToProxy("hello:tcp -h localhost -p 10000"));
        await hello.sayHello();
    }
    catch(ex)
    {
        console.log(ex.toString());
        process.exitCode = 1;
    }
    finally
    {
        if(communicator)
        {
            await communicator.destroy();
        }
    }
})();
