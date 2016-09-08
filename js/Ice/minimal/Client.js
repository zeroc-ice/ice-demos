// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

const Ice = require("ice").Ice;
const Demo = require("./Hello").Demo;
    
let communicator;
 
Ice.Promise.try(() =>
    {
        //
        // Initialize the communicator and create a proxy to the hello object.
        //
        communicator = Ice.initialize(process.argv);
        
        //
        // Down-cast the proxy to the hello object interface and invoke 
        // the sayHello method.
        //
        return Demo.HelloPrx.checkedCast(communicator.stringToProxy("hello:tcp -h localhost -p 10000")).then(
            hello => hello.sayHello());
    }
).finally(() =>
    {
        //
        // Destroy the communicator if required.
        //
        if(communicator)
        {
            return communicator.destroy();
        }
    }
).catch(ex =>
    {
        //
        // Handle any exceptions above.
        //
        console.log(ex.toString());
        process.exit(1);
    });
