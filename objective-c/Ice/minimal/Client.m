// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <objc/Ice.h>
#import <Hello.h>

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
            id<DemoHelloPrx> hello = [DemoHelloPrx checkedCast:[communicator stringToProxy:@"hello:default -p 10000"]];
            if(hello == nil)
            {
                NSLog(@"%s: invalid proxy", argv[0]);
                status = 1;
            }
            else
            {
                [hello sayHello];
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
