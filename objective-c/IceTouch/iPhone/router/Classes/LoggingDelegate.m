// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <LoggingDelegate.h>

@implementation LogEntry

@synthesize type;
@synthesize timestamp;
@synthesize message;
@synthesize category;

-(id)init:(LogEntryType)t message:(NSString*)msg category:(NSString*)cat
{
    if((self = [super init]))
    {
        type = t;
        timestamp = [NSDate date];
        message = msg;
        category = cat;
    }
    return self;
}

+(id)logEntryPrint:(NSString*)message NS_RETURNS_RETAINED
{
    return [[LogEntry alloc] init:LogEntryTypePrint message:message category:0];
}

+(id)logEntryTrace:(NSString*)message category:(NSString*)category NS_RETURNS_RETAINED
{
    return [[LogEntry alloc] init:LogEntryTypeTrace message:message category:category];
}

+(id)logEntryWarning:(NSString*)message NS_RETURNS_RETAINED
{
    return [[LogEntry alloc] init:LogEntryTypeWarning message:message category:0];
}

+(id)logEntryError:(NSString*)message NS_RETURNS_RETAINED
{
    return [[LogEntry alloc] init:LogEntryTypeError message:message category:0];
}

@end
