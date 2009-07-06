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
#import <Cocoa/Cocoa.h>

@interface ButtonConfigController : NSObject {
    IBOutlet NSButton *bClear;
    IBOutlet NSComboBox *cbKey1;
    IBOutlet NSComboBox *cbKey2;
    IBOutlet NSComboBox *cbKey3;
	IBOutlet NSPanel *panel;
	
	int key1;
	int key2;
	int key3;
}

- (NSPanel *)panel;
- (int)key1;
- (void)setKey1:(int)key;
- (int)key2;
- (void)setKey2:(int)key;
- (int)key3;
- (void)setKey3:(int)key;

-(IBAction)useDefault:(id)sender;
- (IBAction)clear:(id)sender;
- (IBAction)close:(id)sender;
- (IBAction)done:(id)sender;
- (void)assignKey:(int)button withKeyCode:(int)keyCode;

@end
