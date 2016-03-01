// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
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
