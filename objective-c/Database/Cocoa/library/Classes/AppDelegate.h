//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class LibraryController;

@interface AppDelegate : NSObject
{
    NSWindowController* connectController;
    LibraryController* libraryController;
    BOOL terminate;
}

-(void)login:(id)sender;
-(void)setLibraryController:(LibraryController*)controller;

@end
