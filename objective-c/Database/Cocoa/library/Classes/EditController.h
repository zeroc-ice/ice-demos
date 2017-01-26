// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <Cocoa/Cocoa.h>

@class DemoBookDescription;

@interface EditController : NSWindowController
{
    IBOutlet NSTextField* isbnField;
    IBOutlet NSTextView* titleField;
    IBOutlet NSTableView* authorsTable;
    
    DemoBookDescription* __weak orig;
    DemoBookDescription* result;
    NSMutableArray* authors;
}

@property (weak, readonly) DemoBookDescription* orig;
@property (readonly) DemoBookDescription* result;

-(id)initWithDesc:(DemoBookDescription*)desc;

-(void)add:(id)sender;
-(void)remove:(id)sender;
-(void)ok:(id)sender;
-(void)cancel:(id)sender;

@end
