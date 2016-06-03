// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <RentController.h>


@implementation RentController

-(id)init
{
    return [super initWithWindowNibName:@"RentView"];
}

-(NSString*)renter
{
    return renterField.stringValue;
}

-(void)ok:(id)sender
{
    [NSApp endSheet:self.window returnCode:1];
}

-(void)cancel:(id)sender
{
    [NSApp endSheet:self.window returnCode:0];
}
@end
