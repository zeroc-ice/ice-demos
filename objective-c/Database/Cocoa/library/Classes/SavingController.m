//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#import <SavingController.h>

@implementation SavingController

-(id)init
{
    return [super initWithWindowNibName:@"SavingView"];
}

-(void)awakeFromNib
{
    [progress startAnimation:self];
}

@end
