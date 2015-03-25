// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// This copy of Ice Touch is licensed to you under the terms described in the
// ICE_TOUCH_LICENSE file included in this distribution.
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
