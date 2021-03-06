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

@interface NewConfigurationWindowController : NSObject {
	IBOutlet NSWindow *window;
    IBOutlet NSButton *bCopyCurrentConfig;
    IBOutlet NSTextField *tfConfigName;
	
	bool copyCurrentConfig;
	NSString *configName;
}

- (NSWindow *)window;
- (bool)copyCurrentConfig;
- (NSString *)configName;

- (void)setup;
- (IBAction)cancel:(id)sender;
- (IBAction)createNewConfiguration:(id)sender;
@end
