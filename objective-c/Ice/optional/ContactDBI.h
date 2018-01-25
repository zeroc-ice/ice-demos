// **********************************************************************
//
// Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
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
