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
#import "ButtonConfigController.h"
#import "ButtonConfigWindow.h"

@implementation ButtonConfigWindow

- (id)init
{
	self = [super init];
	if (self != nil) {
		buttonDownCount = 0;
	}
	return self;
}

// why is there a setup call? cause I don't really know how to program for the mac...
- (void)setup
{
	[[self delegate] clear:self];
	buttonDownCount = 0;
}
	
- (void)keyDown:(NSEvent *)event
{
	int keyCode = [event keyCode];
	if(![event isARepeat]){
		[[self delegate] assignKey:buttonDownCount withKeyCode:keyCode];
		buttonDownCount++;
	}
}

- (void)keyUp:(NSEvent *)event
{
	buttonDownCount--;
	if(buttonDownCount <= 0) {
		buttonDownCount = 0;
		[[self delegate] done:0];
	}
}

- (void)flagsChanged:(NSEvent *)event
{
	int keyCode = [event keyCode];
	if([event modifierFlags] != 256) { // FIXME: I don't know why the magic number is 256
		[[self delegate] assignKey:buttonDownCount withKeyCode:keyCode];
		buttonDownCount++;
	} else {
		buttonDownCount--;
		if(buttonDownCount <= 0) {
			buttonDownCount = 0;
			[[self delegate] done:0];
		}
	}
}

@end
