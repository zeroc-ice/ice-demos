// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <objc/Ice.h>
#import <HelloI.h>

int
main(int argc, char* argv[])
{
    int status = 0;
    @autoreleasepool
    {
        id<ICECommunicator> communicator = nil;
        @try
        {
            communicator = [ICEUtil createCommunicator:&argc argv:argv];
            if(argc > 1)
            {
                NSLog(@"%s: too many arguments", argv[0]);
                return 1;
            }

            id<ICEObjectAdapter> adapter = [communicator createObjectAdapterWithEndpoints:@"Hello"
                                                         endpoints:@"default -p 10000"];
            [adapter add:[HelloI hello] identity:[ICEUtil stringToIdentity:@"hello"]];
            [adapter activate];
            [communicator waitForShutdown];
        }
        @catch(ICELocalException* ex)
        {
            NSLog(@"%@", ex);
            status = 1;
        }

        if(communicator != nil)
        {
            @try
            {
                [communicator destroy];
            }
            @catch(ICELocalException* ex)
            {
                NSLog(@"%@", ex);
                status = 1;
            }
        }
    }
    return status;
}
