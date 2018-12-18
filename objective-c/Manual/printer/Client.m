// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <objc/Ice.h>
#import <Printer.h>
#import <stdio.h>

int
main(int argc, char* argv[])
{
    int status = 1;
    @autoreleasepool
    {
        id<ICECommunicator> communicator = nil;
        @try
        {
            communicator = [ICEUtil createCommunicator:&argc argv:argv];
            id<ICEObjectPrx> base = [communicator stringToProxy:@"SimplePrinter:default -p 10000"];
            id<DemoPrinterPrx> printer = [DemoPrinterPrx checkedCast:base];
            if(!printer)
            {
                [NSException raise:@"Invalid proxy" format:@""];
            }
            [printer printString:@"Hello World!"];
            status = 0;
        }
        @catch(NSException* ex)
        {
            NSLog(@"%@", ex);
        }

        [communicator destroy];
    }
    return status;
}
