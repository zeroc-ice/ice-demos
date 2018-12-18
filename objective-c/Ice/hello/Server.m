// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <objc/Ice.h>
#import <HelloI.h>

int run(id<ICECommunicator>);

int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    ICEregisterIceUDP(YES);
    ICEregisterIceSSL(YES);
    ICEregisterIceWS(YES);
#endif

    int status = 0;
    @autoreleasepool
    {
        id<ICECommunicator> communicator = nil;
        @try
        {
            communicator = [ICEUtil createCommunicator:&argc argv:argv configFile:@"config.server"];
            if(argc > 1)
            {
                NSLog(@"%s: too many arguments", argv[0]);
                status = 1;
            }
            else
            {
                status = run(communicator);
            }
        }
        @catch(ICELocalException* ex)
        {
            NSLog(@"%@", ex);
            status = 1;
        }

        [communicator destroy];
    }
    return status;
}

int
run(id<ICECommunicator> communicator)
{
    id<ICEObjectAdapter> adapter = [communicator createObjectAdapter:@"Hello"];
    [adapter add:[HelloI hello] identity:[ICEUtil stringToIdentity:@"hello"]];
    [adapter activate];
    [communicator waitForShutdown];
    return 0;
}
