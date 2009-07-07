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
#import <time.h>
#import "Server.h"
#import "KeySyms.h"
#import "OS.h"
#import "GamePadConfigView.h"
#import "PadConfigController.h"

using namespace WifiPad;

@implementation PadConfigController

//@synthesize server, model, newConfigWindow, newConfigWindowController, buttonConfigWindow, buttonConfigController, bShowKeys, bShowTapZones, cbConfiguration, cbGamePad, tfServerName;

- (WifiPad::Server *)server
{
	return server;
}

- (void)setServer:(WifiPad::Server *)aServer
{
	server = aServer;
}

- (PadConfigModel *)model
{
	return model;
}

- (NSWindow *)newConfigWindow
{
	return newConfigWindow;
}

- (NewConfigurationWindowController *)newConfigWindowController
{
	return newConfigWindowController;
}

- (ButtonConfigWindow *)buttonConfigWindow
{
	return buttonConfigWindow;
}

- (ButtonConfigController *)buttonConfigController
{
	return buttonConfigController;
}

- (NSButton *)bShowKeys
{
	return bShowKeys;
}

- (NSButton *)bShowTapZones
{
	return bShowTapZones;
}

- (NSComboBox *)cbConfiguration
{
	return cbConfiguration;
}

- (NSComboBox *)cbGamePad
{
	return cbGamePad;
}

- (NSTextField *)tfServerName
{
	return tfServerName;
}

- (GamePadConfigView *)view
{
	return view;
}

- (void)awakeFromNib
{
	try {
		char hostname[256];
		gethostname(hostname,255);
		hostname[255] = 0;
		server = new WifiPad::Server("0.0.0.0",8989);
	} catch(const std::runtime_error& e) {
		NSAlert *alert = [[NSAlert alloc] init];
		[alert setMessageText:[NSString stringWithUTF8String:e.what()]];
		[alert runModal];
		[alert release]; 
		exit(1);
	}
	
	model = new PadConfigModel();
	
	[self reloadGamePadList];
	
	[self reloadConfigurationList];
	
	server->Start();
}

- (void)reloadConfigurationList
{
	[cbConfiguration removeAllItems];
	server->GetConfigurationList(&model->configurationList);
	[cbConfiguration addItemWithObjectValue:@"<Default configuration>"];
	for(Server::ConfigurationList::const_iterator it =
		model->configurationList.begin(); it != model->configurationList.end(); ++it) {
		[cbConfiguration addItemWithObjectValue:[NSString stringWithUTF8String:it->c_str()]];
	}
	[cbConfiguration selectItemAtIndex:0];
	[view refresh:self];
}

- (void)reloadGamePadList
{
	[cbGamePad removeAllItems];
	server->LoadGamePads();
	server->GetGamePadList(&model->gamePadList);
	for(Server::GamePadList::const_iterator it =
		model->gamePadList.begin(); it != model->gamePadList.end(); ++it) {
		[cbGamePad addItemWithObjectValue:[NSString stringWithUTF8String:it->gamePad->GetName().c_str()]];
	}
	[cbGamePad selectItemAtIndex:0];
	[view refresh:self];
}

- (void)setupButtonConfig:(int)buttonNo
{
	if(!model->configuration) {
		NSAlert *alert = [[NSAlert alloc] init];
		[alert setMessageText:@"To configure keys, please create or select a configuration."];
		[alert runModal];
		[alert release]; 
		return;
	}

	[buttonConfigWindow setup];
	int code = [NSApp runModalForWindow:buttonConfigWindow];
	if(code == 0) { // sucessful button entry
		ConfigurationKey pkey;
		ConfigurationValue pvalue;
		
		pkey.gamepadUUID = model->gamePad->GetUuid();
		pkey.buttonNo = buttonNo;
		
		if(!model->configuration->GetValue(pkey,&pvalue)) {
			ButtonInfo& button = model->gamePad->GetButton(buttonNo);
			pvalue.key[0] = button.defaultKey[0];
			pvalue.key[1] = button.defaultKey[1];
			pvalue.key[2] = button.defaultKey[2];
		}
		

		pvalue.key[0] = [buttonConfigController key1];
		pvalue.key[1] = [buttonConfigController key2];
		pvalue.key[2] = [buttonConfigController key3];
			
		model->configuration->SetValue(pkey,pvalue);
	} else if(code == 1) { // delete config entry
		ConfigurationKey pkey;
		
		pkey.gamepadUUID = model->gamePad->GetUuid();
		pkey.buttonNo = buttonNo;
		
		model->configuration->UnsetValue(pkey);
	}
	[view refresh:self];
}

- (IBAction)deleteConfiguration:(id)sender {
    try {
		int selected = [cbConfiguration indexOfSelectedItem];
		if(selected <= 0) throw std::runtime_error("You cannot delete the default configuration.");
		
		NSAlert *alert = [[NSAlert alloc] init];
		[alert setAlertStyle:NSInformationalAlertStyle];
		[alert addButtonWithTitle:@"No"];
		[alert addButtonWithTitle:@"Yes"];
		[alert setMessageText:[NSString stringWithUTF8String:("Do you wish to delete the configuration \"" + model->configurationList[selected - 1] + "\"?").c_str()]];
		int code = [alert runModal];
		[alert release]; 
		if(code == NSAlertSecondButtonReturn) {
			OS::DeleteFile(server->GetConfigurationsPath() + "/" + model->configurationList[selected - 1] + ".config");
			alert = [[NSAlert alloc] init];
			[alert setAlertStyle:NSInformationalAlertStyle];
			[alert setMessageText:@"Configuration deleted."];
			[alert runModal];
			[alert release]; 
			[self reloadConfigurationList];
		}
	} catch(const std::runtime_error& e) {
		NSAlert *alert = [[NSAlert alloc] init];
		[alert setMessageText:[NSString stringWithUTF8String:e.what()]];
		[alert runModal];
		[alert release]; 
	}
}

- (IBAction)newConfiguration:(id)sender {
	[newConfigWindowController setup];
    int code = [NSApp runModalForWindow:newConfigWindow];
	if(code == 0) {
		try {
			const char * configName = [[newConfigWindowController configName] UTF8String];
			// check if configuration exists first
			if(OS::FileExists(server->GetConfigurationsPath() + "/" + configName + ".config")) {
				throw std::runtime_error("Could not create configuration because configuration already exists.");
			}
			
			ConfigurationPtr config(new Configuration(configName));
			// copy configuration if checked
			if([newConfigWindowController copyCurrentConfig]) {
				if(model->configuration) {
					*config = *model->configuration;
				}
			}
		
			server->SaveConfiguration(config,configName);
			// reload configuration
			[self reloadConfigurationList];
		
		} catch(const std::runtime_error& e) {
			NSAlert *alert = [[NSAlert alloc] init];
			[alert setMessageText:[NSString stringWithUTF8String:e.what()]];
			[alert runModal];
			[alert release]; 
		}
	}
}

- (IBAction)saveConfiguration:(id)sender {
	try {
		int selected = [cbConfiguration indexOfSelectedItem];
		if(selected <= 0) throw std::runtime_error("You cannot save over the default configuration.");
		server->SaveConfiguration(model->configuration,model->configurationList[selected - 1]);
		
		// reload configurations for each device
		Server::DeviceList devices;
		server->GetDeviceList(&devices);
		for(Server::DeviceList::iterator device = devices.begin(); device != devices.end(); ++device)
		{
			ConfigurationPtr config = (*device)->GetConfiguration();
			if(config && config->GetName() == model->configurationList[selected - 1]) {
				(*device)->SetConfiguration(server->LoadConfiguration(model->configurationList[selected - 1]));
			}
		}
		
		NSAlert *alert = [[NSAlert alloc] init];
		[alert setAlertStyle:NSInformationalAlertStyle];
		[alert setMessageText:@"Configuration saved."];
		[alert runModal];
		[alert release]; 
	} catch(const std::runtime_error& e) {
		NSAlert *alert = [[NSAlert alloc] init];
		[alert setMessageText:[NSString stringWithUTF8String:e.what()]];
		[alert runModal];
		[alert release]; 
	}
}

- (IBAction)installGamePad:(id)sender
{
	NSOpenPanel *fileDlg = [NSOpenPanel openPanel];
	[fileDlg setCanChooseFiles:YES];
	[fileDlg setAllowsMultipleSelection:NO];
	[fileDlg setCanChooseDirectories:NO];
	[fileDlg setTitle:@"Choose a gamepad to install..."];
	if([fileDlg runModalForDirectory:nil file:nil] == NSOKButton) 
	{
		try {
			NSArray *filenames = [fileDlg filenames];
			NSString *file = [filenames objectAtIndex:0];
			std::string filebase = [file UTF8String];
			size_t n = filebase.find_last_of('/');
			if(n != std::string::npos) {
				filebase = filebase.substr(n + 1);
			} 
		
			// load gamepad to make sure it's valid
			GamePadPtr checkPad(new GamePad());
			try {
				FileDataSource source([file UTF8String]);
				checkPad->LoadGamePad(source,0);
			} catch(const std::runtime_error& e) {
				throw std::runtime_error(std::string("Failed to install gamepad. The selected gamepad is invalid. Reason: ") + e.what());
			}
			
			// check for duplicate uuid
			try {
				server->GetGamePadPathByUuid(checkPad->GetUuid());
				NSAlert *alert = [[NSAlert alloc] init];
				[alert setMessageText:@"This gamepad is already installed. (Or matches a UUID of an existing gamepad.)"];
				[alert runModal];
				[alert release]; 
				return;
			} catch(const std::runtime_error&) {
			}
			
			char curtime[32];
			sprintf(curtime,"%ld",(long)time(NULL));
			OS::CopyFile([file UTF8String],server->GetGamePadsPath() + "/" + curtime + "." + filebase);
			
			NSAlert *alert = [[NSAlert alloc] init];
			[alert setAlertStyle:NSInformationalAlertStyle];
			[alert setMessageText:@"Gamepad installed."];
			[alert runModal];
			[alert release]; 
			
			[self reloadGamePadList];
		} catch(const std::runtime_error& e) {
			NSAlert *alert = [[NSAlert alloc] init];
			[alert setMessageText:[NSString stringWithUTF8String:e.what()]];
			[alert runModal];
			[alert release]; 
		}
	}
}

- (IBAction)deleteGamePad:(id)sender
{
	try {
		int selected = [cbGamePad indexOfSelectedItem];
		if(selected <= 0) throw std::runtime_error("You cannot delete the default gamepad.");
		
		NSAlert *alert = [[NSAlert alloc] init];
		[alert setAlertStyle:NSInformationalAlertStyle];
		[alert addButtonWithTitle:@"No"];
		[alert addButtonWithTitle:@"Yes"];
		[alert setMessageText:[NSString stringWithUTF8String:("Do you want to delete the gamepad \"" + model->gamePadList[selected].gamePad->GetName() + "\"?").c_str()]];
		int code = [alert runModal];
		[alert release]; 
		if(code == NSAlertSecondButtonReturn) {
			OS::DeleteFile(model->gamePadList[selected].path);
			[self reloadGamePadList];
		}
	} catch(const std::runtime_error& e) {
		NSAlert *alert = [[NSAlert alloc] init];
		[alert setMessageText:[NSString stringWithUTF8String:e.what()]];
		[alert runModal];
		[alert release]; 
	}
}

- (void)controlTextDidChange:(NSNotification *)aNotification
{
	if([aNotification object] == tfServerName) {
		server->SetServerName([[tfServerName stringValue] UTF8String]);
	}
}

- (void)comboBoxSelectionDidChange:(NSNotification *)notification
{
	if([notification object] == cbGamePad)
	{
		int selected = [cbGamePad indexOfSelectedItem];
		if(selected >= 0) {
			try {
				model->gamePad = server->LoadGamePadByUuid(model->gamePadList[selected].gamePad->GetUuid());
			} catch(const std::runtime_error& e) {
				NSAlert *alert = [[NSAlert alloc] init];
				[alert setMessageText:[NSString stringWithUTF8String:e.what()]];
				[alert runModal];
				[alert release];
				//[self reloadGamePadList];
			}
		} else {
			model->gamePad.reset();
		}
		[view setNeedsDisplay:TRUE];
	} else if([notification object] == cbConfiguration) {
		int selected = [cbConfiguration indexOfSelectedItem];
		if(selected > 0) {
			try {
				model->configuration = server->LoadConfiguration(model->configurationList[selected - 1]);
			} catch(const std::runtime_error& e) {
				NSAlert *alert = [[NSAlert alloc] init];
				[alert setMessageText:[NSString stringWithUTF8String:e.what()]];
				[alert runModal];
				[alert release];
				[self reloadConfigurationList];
			}
		} else {
			model->configuration.reset();
		}
	}
	[view refresh:self];
}

- (IBAction)setSensitivity:(id)sender
{
	int speed = [sMouseSpeed intValue];
	server->SetMouseSpeed(speed);
}

- (void) dealloc
{
	server->Stop();
	delete model;
	delete server;
	[super dealloc];
}

@end
