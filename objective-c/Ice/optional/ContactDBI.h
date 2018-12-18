// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

#import <Contact.h>

@interface ContactDBI : DemoContactDB<DemoContactDB>
{
@private
    NSMutableDictionary* contacts_;
}
@end
