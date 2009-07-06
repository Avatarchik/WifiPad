/*
 This file is part of WifiPad.
 Copyright (c) 2009 Sound Tang, Victor Chow
 Website: http://www.wifipad.com/
 
 WifiPad is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 WifiPad is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with WifiPad.  If not, see <http://www.gnu.org/licenses/>.
 */
#import <string>
#import "KeySyms.h"
#import "ButtonConfigController.h"

using namespace WifiPad;

@implementation ButtonConfigController

- (NSPanel *)panel
{
	return panel;
}

- (int)key1
{
	return key1;
}

- (void)setKey1:(int)key
{
	key1 = key;
}

- (int)key2
{
	return key2;
}

- (void)setKey2:(int)key
{
	key2 = key;
}

- (int)key3
{
	return key3;
}

- (void)setKey3:(int)key
{
	key3 = key;
}

- (void)awakeFromNib
{
	NSComboBox *c[] = { cbKey1, cbKey2, cbKey3, NULL };
	for(int i = 0; c[i]; i++) {
		[c[i] addItemWithObjectValue:@""];
		[c[i] addItemWithObjectValue:@"MOUSEUP"];
		[c[i] addItemWithObjectValue:@"MOUSEDOWN"];
		[c[i] addItemWithObjectValue:@"MOUSELEFT"];
		[c[i] addItemWithObjectValue:@"MOUSERIGHT"];
		[c[i] addItemWithObjectValue:@"WHEELUP"];
		[c[i] addItemWithObjectValue:@"WHEELDOWN"];
		[c[i] addItemWithObjectValue:@"LBUTTON"];
		[c[i] addItemWithObjectValue:@"RBUTTON"];
		[c[i] addItemWithObjectValue:@"MBUTTON"];
	}
}

-(IBAction)clear:(id)sender
{
	self.key1 = self.key2 = self.key3 = -1;
	[cbKey1 setStringValue:@""];
	[cbKey2 setStringValue:@""];
	[cbKey3 setStringValue:@""];
}

-(IBAction)useDefault:(id)sender
{
	[NSApp stopModalWithCode:1];
	[panel close];
}

-(IBAction)close:(id)sender
{
	[NSApp stopModalWithCode:-1];
	[panel close];
}

- (IBAction)done:(id)sender
{
	self.key1 = KeySyms::MapSymbol([[cbKey1 stringValue] UTF8String]);
	self.key2 = KeySyms::MapSymbol([[cbKey2 stringValue] UTF8String]);
	self.key3 = KeySyms::MapSymbol([[cbKey3 stringValue] UTF8String]);
	[NSApp stopModalWithCode:0];
	[panel close];
}

- (void)assignKey:(int)button withKeyCode:(int)keyCode
{
	std::string key;
	key = KeySyms::MapKey(keyCode);
	switch(button) {
		case 0:
			[cbKey1 setStringValue:[NSString stringWithUTF8String:key.c_str()]];
			break;
		case 1:
			[cbKey2 setStringValue:[NSString stringWithUTF8String:key.c_str()]];
			break;
		case 2:
			[cbKey3 setStringValue:[NSString stringWithUTF8String:key.c_str()]];
			break;
	}
}

- (void)comboBoxSelectionDidChange:(NSNotification *)notification
{
	if([notification object] == cbKey1) {
		
	}
}

- (BOOL)acceptsFirstResponder
{
	return YES;
}
	
- (void) dealloc
{
	[super dealloc];
}


@end
