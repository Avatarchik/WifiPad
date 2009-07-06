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
#import <stdexcept>
#import "NewConfigurationWindowController.h"

@implementation NewConfigurationWindowController

- (NSWindow *)window
{
	return window;
}

- (bool)copyCurrentConfig
{
	return copyCurrentConfig;
}

- (NSString *)configName
{
	return configName;
}

- (void)setup
{
	[tfConfigName setStringValue:@""];
}

- (IBAction)cancel:(id)sender {
    [NSApp stopModalWithCode:-1];
	[window close];
}

- (IBAction)createNewConfiguration:(id)sender {
    // verify some fields
	try {
		NSString *config = [tfConfigName stringValue];
		if([config isEqualToString:@""]) {
			throw std::runtime_error("Please enter a configuration name.");
		}
		if([config length] > 31) {
			throw std::runtime_error("Configuration name must be less than 32 characters.");
		}
		
		// check for funny characters
		const char *c = [config UTF8String];
		while(*c) {
			if(!isprint(*c) ||
			   *c == '/' || *c == '\\' || *c == ':' ||
			   *c == '*' || *c == '?' || *c == '"' ||
			   *c == '<' || *c == '>' || *c == '|') {
				throw std::runtime_error("Configuration name cannot contain the follow characters: / \\ : * ? \" < > |");
			}
			++c;
		}
		
		configName = config;
		copyCurrentConfig = [bCopyCurrentConfig state] == NSOnState;

		[NSApp stopModalWithCode:0];
		[window close];
	} catch(const std::runtime_error& e) {
		NSAlert *alert = [[NSAlert alloc] init];
		[alert setMessageText:[NSString stringWithUTF8String:e.what()]];
		[alert runModal];
		[alert release]; 
	}
}

- (void) dealloc
{
	[super dealloc];
}

@end
