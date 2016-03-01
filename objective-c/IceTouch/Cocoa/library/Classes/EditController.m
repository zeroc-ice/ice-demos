// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#import <EditController.h>
#import <Library.h>

@implementation EditController

@synthesize orig;
@synthesize result;

-(id)initWithDesc:(DemoBookDescription*)desc
{
    if(self = [super initWithWindowNibName:@"EditView"])
    {
        orig = desc;
    }
    return self;
}

-(void)awakeFromNib
{
    if(orig)
    {
        isbnField.stringValue = orig.isbn;
        [[titleField textStorage] setAttributedString:[[NSAttributedString alloc] initWithString:orig.title]];
        authors = [orig.authors mutableCopy];
        [isbnField setEditable:NO];
    }
    else
    {
        [isbnField setEditable:YES];
        [isbnField becomeFirstResponder];
        isbnField.stringValue = @"";
        [[titleField textStorage] setAttributedString:[[NSAttributedString alloc] initWithString:@""]];
        authors = [NSMutableArray array];
    }
    [authorsTable reloadData];
}

-(void)add:(id)sender
{
    [authors addObject:@""];
    [authorsTable reloadData];

    // Immediately start editing the author.
    [authorsTable editColumn:0 row:authors.count-1  withEvent:nil select:YES]; 
}

-(void)remove:(id)sender
{
    [authors removeObjectAtIndex:authorsTable.selectedRow];
    [authorsTable reloadData];
}

-(void)ok:(id)sender
{
    if(orig)
    {
        result = [orig copy];
    }
    else
    {
        result = [[DemoBookDescription alloc] init];
    }
    result.title = [[titleField textStorage] string];
    result.isbn = isbnField.stringValue;
    NSMutableArray* c = [NSMutableArray array];
    for(NSString* a in authors)
    {
        NSString* trimmed = [a stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
        if(trimmed.length > 0)
        {
            [c addObject:trimmed];
        }
    }
    result.authors = c;
    
    [NSApp endSheet:self.window returnCode:1];
}

-(void)cancel:(id)sender
{
    [NSApp endSheet:self.window returnCode:0];
}

#pragma mark NSTableView delgate

- (BOOL)control:(NSControl*)control textShouldEndEditing:(NSText*)fieldEditor
{
    // The field text must be copied.
    [authors replaceObjectAtIndex:[authorsTable editedRow] withObject:[fieldEditor.string copy]];
    return YES;
}

#pragma mark NSTableViewDataSource methods

- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
    return authors.count;
}

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
    return [authors objectAtIndex:rowIndex];
}

@end
