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
#import "GamePad.h"
#import "Image.h"
#import "KeySyms.h"
#import "GamePadConfigView.h"

using namespace WifiPad;

@implementation GamePadConfigView

// accessors
- (PadConfigController *)padConfigController
{
	return padConfigController;
}

- (void)setPadConfigController:(PadConfigController *)controller
{
	padConfigController = controller;
}

- (void)awakeFromNib
{
	selectedButton = -1;
	[[self window] setAcceptsMouseMovedEvents:YES];
}

- (void)drawTapZone:(GamePad *)gamePad withButtonNo:(int)buttonNo
{
	ButtonInfo& button = gamePad->GetButton(buttonNo);
	
	NSRect rect;
	rect.origin.x = button.x;
	rect.origin.y = 320 - button.y - button.height;
	rect.size.width = button.width;
	rect.size.height = button.height;
	
	NSColor *color = [NSColor colorWithDeviceRed:0.0f green:0.0f blue:1.0f alpha:0.5];
	[color setFill];
	NSRectFillUsingOperation(rect,NSCompositeSourceAtop);
}

- (void)drawRect:(NSRect)rect
{
	NSGraphicsContext *currentGraphicsContext = [NSGraphicsContext currentContext]; 
	[NSGraphicsContext saveGraphicsState];
	
	CGContextRef context = (CGContextRef)[currentGraphicsContext graphicsPort];
	
	GamePadPtr gamePad = [padConfigController model]->gamePad;
	ConfigurationPtr configuration = [padConfigController model]->configuration;
	if(gamePad) {
		ImagePtr gamepadImage = gamePad->GetMainImage();
		
		if(gamepadImage)
			CGContextDrawImage(context,CGRectMake(0,0,gamepadImage->GetWidth(),gamepadImage->GetHeight()),*gamepadImage);
		
		// get checked state of tap zones
		bool showTapZones = [[padConfigController bShowTapZones] state] == NSOnState;
		bool showKeys = [[padConfigController bShowKeys] state] == NSOnState;
		
		for(int i = 0; i < gamePad->GetNumButtons(); i++) {
			NSRect rect;
			ButtonInfo& button = gamePad->GetButton(i);
			rect.origin.x = button.x;
			rect.origin.y = 320 - button.y - button.height;
			rect.size.width = button.width;
			rect.size.height = button.height;
			
			// blit tap zone
			if(showTapZones || selectedButton == i)
			{
				NSColor *color;
				if(button.isTrackPad) color = [NSColor colorWithDeviceRed:1.0f green:0.0f blue:0.0f alpha:0.5];
				else color = [NSColor colorWithDeviceRed:0.0f green:0.0f blue:1.0f alpha:0.5];
				[color setFill];
				NSRectFillUsingOperation(rect,NSCompositeSourceAtop);
			}
			// blit keynames
			if(showKeys) {
				// displays keys for the given device's configuration
				ConfigurationKey pkey;
				ConfigurationValue pvalue;
				int keyCode[3];
				
				pkey.gamepadUUID = gamePad->GetUuid();
				pkey.buttonNo = i;
				
				if(configuration && configuration->GetValue(pkey,&pvalue)) {
					keyCode[0] = pvalue.key[0];
					keyCode[1] = pvalue.key[1];
					keyCode[2] = pvalue.key[2];
				} else {
					keyCode[0] = button.defaultKey[0];
					keyCode[1] = button.defaultKey[1];
					keyCode[2] = button.defaultKey[2];
				}
				
				std::string key = (keyCode[0] >= 0 ? KeySyms::MapKey(keyCode[0]) : "") +
					(keyCode[1] >= 0 ? "+" + KeySyms::MapKey(keyCode[1]) : "") +
					(keyCode[2] >= 0 ? "+" + KeySyms::MapKey(keyCode[2]) : "");
				
				NSString *nsStr = [NSString stringWithUTF8String:key.c_str()];
				NSDictionary *attr = [NSDictionary dictionaryWithObject:[NSColor colorWithDeviceRed:1.0f green:1.0f blue:1.0f alpha:0.8f] forKey:NSBackgroundColorAttributeName];
				NSSize halfSize = [nsStr sizeWithAttributes:attr];
				halfSize.width /= 2;
				halfSize.height /= 2;
				NSPoint pt;
				pt.x = rect.origin.x + (rect.size.width / 2) - halfSize.width;
				pt.y = rect.origin.y + (rect.size.height / 2) - halfSize.height;
				[nsStr drawAtPoint:pt withAttributes:attr];	
			}
		}
	}
	
	[NSGraphicsContext restoreGraphicsState];
}

- (void)mouseMoved:(NSEvent *)event
{
	GamePadPtr gamePad = [padConfigController model]->gamePad;
	if(!gamePad) return;
	
	NSPoint windowPoint = [event locationInWindow];
	NSPoint mousePoint = [self convertPoint:windowPoint fromView:nil];
	
	mousePoint.y = 320 - mousePoint.y;
	// blit-tap zones
	for(int i = 0; i < gamePad->GetNumButtons(); i++) {
		ButtonInfo& button = gamePad->GetButton(i);
		
		NSRect rect;
		rect.origin.x = button.x;
		rect.origin.y = button.y; // reverse coordinates for quartz
		rect.size.width = button.width;
		rect.size.height = button.height;
		if(NSMouseInRect(mousePoint,rect,TRUE)) { 
			selectedButton = i;
			[self setNeedsDisplay:TRUE];
			return;
		}
	}
	
	if(selectedButton != -1) {
		selectedButton = -1;
		[self setNeedsDisplay:TRUE];
	}
}

- (void)mouseDown:(NSEvent *)event
{
	GamePadPtr gamePad = [padConfigController model]->gamePad;
	if(!gamePad) return;
	
	NSPoint windowPoint = [event locationInWindow];
	NSPoint mousePoint = [self convertPoint:windowPoint fromView:nil];
	
	mousePoint.y = 320 - mousePoint.y;
	if(selectedButton != -1) {
		ButtonInfo& button = gamePad->GetButton(selectedButton);
		NSRect rect;
		rect.origin.x = button.x;
		rect.origin.y = button.y; // reverse coordinates for quartz
		rect.size.width = button.width;
		rect.size.height = button.height;
		if(NSMouseInRect(mousePoint,rect,TRUE)) { 
			[padConfigController setupButtonConfig:selectedButton];
		}
	}
}

- (IBAction)refresh:(id)sender
{
	[self setNeedsDisplay:YES];
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
