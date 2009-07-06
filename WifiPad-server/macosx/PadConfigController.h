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
#import "NewConfigurationWindowController.h"
#import "ButtonConfigWindow.h"
#import "ButtonConfigController.h"
#import "GamePad.h"
#import "Configuration.h"
#import "Server.h"

struct PadConfigModel
{
	WifiPad::Server::GamePadList gamePadList;
	WifiPad::Server::ConfigurationList configurationList;
	WifiPad::GamePadPtr gamePad; // current gamepad
	WifiPad::ConfigurationPtr configuration;
};

@class GamePadConfigView;

@interface PadConfigController : NSObject {
	WifiPad::Server *server;
	PadConfigModel *model; // current gamepad
	IBOutlet NSWindow *newConfigWindow;
	IBOutlet NewConfigurationWindowController *newConfigWindowController;
	IBOutlet ButtonConfigWindow *buttonConfigWindow;
	IBOutlet ButtonConfigController *buttonConfigController;
    IBOutlet NSButton *bShowKeys;
    IBOutlet NSButton *bShowTapZones;
    IBOutlet NSComboBox *cbConfiguration;
    IBOutlet NSComboBox *cbGamePad;
    IBOutlet NSTextField *tfServerName;
	IBOutlet NSSlider *sMouseSpeed;
	IBOutlet GamePadConfigView *view;
}

- (WifiPad::Server *)server;
- (void)setServer:(WifiPad::Server *)aServer;
- (PadConfigModel *)model;
- (NSWindow *)newConfigWindow;
- (NewConfigurationWindowController *)newConfigWindowController;
- (ButtonConfigWindow *)buttonConfigWindow;
- (ButtonConfigController *)buttonConfigController;
- (NSButton *)bShowKeys;
- (NSButton *)bShowTapZones;
- (NSComboBox *)cbConfiguration;
- (NSComboBox *)cbGamePad;
- (NSTextField *)tfServerName;
- (GamePadConfigView *)view;

- (void)reloadConfigurationList;
- (void)reloadGamePadList;
- (void)setupButtonConfig:(int)buttonNo;
- (IBAction)deleteConfiguration:(id)sender;
- (IBAction)newConfiguration:(id)sender;
- (IBAction)saveConfiguration:(id)sender;
- (IBAction)installGamePad:(id)sender;
- (IBAction)deleteGamePad:(id)sender;
- (IBAction)setSensitivity:(id)sender;
@end
