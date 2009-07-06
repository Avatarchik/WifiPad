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
#import "ButtonConfigWindow.h"
#import "PadConfigController.h"

@interface GamePadConfigView : NSView {
	IBOutlet PadConfigController *padConfigController;
	int selectedButton;
}

- (PadConfigController *)padConfigController;
- (void)setPadConfigController:(PadConfigController *)controller;

- (IBAction)refresh:(id)sender;

@end
