// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <objc/Ice.h>
#import <Printer.h>
#import <stdio.h>

@interface PrinterI : DemoPrinter <DemoPrinter>
@end

@implementation PrinterI
-(void) printString:(NSMutableString *)s current:(ICECurrent *)current
{
    printf("%s\n", [s UTF8String]);
    fflush(stdout);
}
@end

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
            id<ICEObjectAdapter> adapter = [communicator createObjectAdapterWithEndpoints:@"SimplePrinterAdapter"
                                                                                endpoints:@"default -p 10000"];
            [adapter add:[PrinterI printer] identity:[ICEUtil stringToIdentity:@"SimplePrinter"]];
            [adapter activate];

            [communicator waitForShutdown];

            status = 0;
        }
        @catch (NSException* ex)
        {
            NSLog(@"%@", ex);
        }

        [communicator destroy];
    }
    return status;
}
