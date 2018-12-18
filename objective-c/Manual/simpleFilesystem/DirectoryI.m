// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <objc/Ice.h>
#include <DirectoryI.h>

@implementation DirectoryI

@synthesize myName;
@synthesize parent;
@synthesize ident;
@synthesize contents;

+(id) directoryi:(NSString *)name parent:(DirectoryI *)parent
{
    DirectoryI *instance = [DirectoryI directory];
    if(instance == nil)
    {
        return nil;
    }
    instance.myName = name;
    instance.parent = parent;
    instance.ident = [ICEIdentity identity:(parent ? [ICEUtil generateUUID] : @"RootDir") category:nil];
    instance.contents = [[NSMutableArray alloc] init];
    return instance;
}

-(NSString *) name:(ICECurrent *)current
{
    return myName;
}

-(NSArray *) list:(ICECurrent *)current
{
    return contents;
}

-(void) addChild:(id<FSNodePrx>)child
{
     [contents addObject:child];
}

-(void) activate:(id<ICEObjectAdapter>)a
{
    id<FSNodePrx> thisNode = [FSNodePrx uncheckedCast:[a add:self identity:ident]];
    [parent addChild:thisNode];
}
@end
