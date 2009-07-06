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
#include "stdafx.h"
#include <windows.h>
#include <string>
#include <map>
#include "resource.h"
#include "Server.h"
#include "OS.h"
#include "KeySyms.h"
#include "GamePad.h"
#include "ButtonConfigDialog.h"
#include "PadConfigDialog.h"

namespace WifiPad
{
	void PadConfigDialog::ReloadConfigurationList()
	{
		m_cbConfiguration.ResetContent();
		m_cbConfiguration.AddString(_T("<Default configuation>"));
		m_server.GetConfigurationList(&m_configurationList);
		for(Server::ConfigurationList::const_iterator it = m_configurationList.begin(); it != m_configurationList.end(); ++it) {
			m_cbConfiguration.AddString(it->c_str());
		}

		if(m_selectedConfiguration < 0 || m_selectedConfiguration >= (int)m_configurationList.size()) {
			m_cbConfiguration.SetCurSel(0);
			m_selectedConfiguration = -1;
			m_configuration.reset();
		} else {
			m_cbConfiguration.SetCurSel(m_selectedConfiguration + 1);
			m_configuration = m_server.LoadConfiguration(m_configurationList[m_selectedConfiguration]);
		}
		RECT rect;
		rect.left = IMAGE_X;
		rect.top = IMAGE_Y;
		rect.right = rect.left + 480;
		rect.bottom = rect.top + 320;
		InvalidateRect(&rect);
	}

	void PadConfigDialog::ReloadGamePadList()
	{
		m_server.GetGamePadList(&m_gamepadList);
		m_cbGamePad.ResetContent();
		for(Server::GamePadList::const_iterator it = m_gamepadList.begin(); it != m_gamepadList.end(); ++it) {
			m_cbGamePad.AddString(it->gamePad->GetName().c_str());
		}
		if(m_selectedGamepad < (int)m_gamepadList.size()) {
			try {
				m_cbGamePad.SetCurSel(m_selectedGamepad);
				FileDataSource source(m_gamepadList[m_selectedGamepad].path);
				m_gamepad.LoadGamePad(source);
			} catch(const std::exception& e) {
				MessageBox(e.what(),_T(("Error loading gamepad: " + m_gamepadList[m_selectedGamepad].path).c_str()),MB_ICONWARNING | MB_OK);
			}
		} else {
			m_selectedGamepad = 0;
		}
		m_cbGamePad.SetCurSel(m_selectedGamepad);
		RECT rect;
		rect.left = IMAGE_X;
		rect.top = IMAGE_Y;
		rect.right = rect.left + 480;
		rect.bottom = rect.top + 320;
		InvalidateRect(&rect);
	}

	LRESULT PadConfigDialog::OnInitDialog(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		m_eServerName = GetDlgItem(IDC_SERVER_NAME);
		m_cbConfiguration = GetDlgItem(IDC_CONFIGURATION);
		m_cbGamePad = GetDlgItem(IDC_GAMEPAD);
		m_bNewConfiguration = GetDlgItem(IDC_NEW_CONFIGURATION);
		m_bDeleteConfiguration = GetDlgItem(IDC_DELETE_CONFIGURATION);
		m_bSaveConfiguration = GetDlgItem(IDC_SAVE_CONFIGURATION);
		m_bShowKeys = GetDlgItem(IDC_SHOW_KEYS);
		m_bShowTapZones = GetDlgItem(IDC_SHOW_TAP_ZONES);
		m_tbMouseSpeed = GetDlgItem(IDC_MOUSE_SPEED);

		m_tbMouseSpeed.SetRange(1,20,TRUE);
		m_tbMouseSpeed.SetPos(m_server.GetMouseSpeed());

		// loads the configurations
		ReloadConfigurationList();

		// loads the gamepad list
		ReloadGamePadList();

		m_bShowKeys.SetCheck(BST_CHECKED);

		m_mouseOverTarget = -1;
		return 0;
	}

	LRESULT PadConfigDialog::OnChangeGamePad(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		m_selectedGamepad = SendDlgItemMessage(IDC_GAMEPAD,CB_GETCURSEL);

		m_mouseOverTarget = -1;
		m_gamepad.Clear();
		if(!m_gamepadList.empty() && m_selectedGamepad >= 0) {
			try {
				m_gamepad.LoadGamePad(FileDataSource(m_gamepadList[m_selectedGamepad].path));
			} catch(const std::exception& e) {
				MessageBox(e.what(),_T(("Error loading gamepad: " + m_gamepadList[m_selectedGamepad].path).c_str()),MB_ICONWARNING | MB_OK);
				// if there was an error loading gamepad, refresh the list
				m_server.LoadGamePads();
				ReloadGamePadList();
			}
		}

		// refresh view
		RECT rect;
		rect.left = IMAGE_X;
		rect.top = IMAGE_Y;
		rect.right = rect.left + 480;
		rect.bottom = rect.top + 320;
		InvalidateRect(&rect);
		return 0;
	}

	LRESULT PadConfigDialog::OnChangeMouseSpeed(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		m_server.SetMouseSpeed(m_tbMouseSpeed.GetPos());
		return 0;
	}

	LRESULT PadConfigDialog::OnPaint(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		PAINTSTRUCT ps;
		CDCHandle dc(BeginPaint(&ps));

		// blit main gamepad image
		ImagePtr gamepadImage = m_gamepad.GetMainImage();

		if(gamepadImage)
			dc.BitBlt(IMAGE_X,IMAGE_Y,gamepadImage->GetWidth(),gamepadImage->GetHeight(),gamepadImage->GetDC(),0,0,SRCCOPY);

		// get checked state of tap zones
		bool showTapZones = m_bShowTapZones.GetCheck() == BST_CHECKED;
		bool showKeys = m_bShowKeys.GetCheck() == BST_CHECKED;

		// blit-tap zones
		CBrush bluebrush, redbrush;
		bluebrush.CreateSolidBrush(RGB(0,0,255));
		redbrush.CreateSolidBrush(RGB(255,0,0));
		HBRUSH oldBrush = dc.SelectBrush(bluebrush);
		for(int i = 0; i < m_gamepad.GetNumButtons(); i++) {
			ButtonInfo& button = m_gamepad.GetButton(i);

			RECT rect;
			rect.left = IMAGE_X + button.x;
			rect.top = IMAGE_Y + button.y;
			rect.right = IMAGE_X + button.x + button.width;
			rect.bottom = IMAGE_Y + button.y + button.height;
			if(showTapZones || m_mouseOverTarget == i) {
				if(button.isTrackPad)
					dc.FillRect(&rect,redbrush);
				else
					dc.FillRect(&rect,bluebrush);
			}

			if(showKeys) {
				// displays keys for the given device's configuration
				ConfigurationKey pkey;
				ConfigurationValue pvalue;
				int keyCode[3];
				
				pkey.gamepadUUID = m_gamepadList[m_selectedGamepad].gamePad->GetUuid();
				pkey.buttonNo = i;

				if(m_configuration && m_configuration->GetValue(pkey,&pvalue)) {
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
			
				SIZE size;
				::GetTextExtentPoint(dc,key.c_str(),key.size(),&size);
				dc.TextOut((rect.left + rect.right - size.cx) >> 1,
					(rect.top + rect.bottom - size.cy) >> 1,key.c_str(),key.size());
			}
		}
		dc.SelectBrush(oldBrush);

		EndPaint(&ps);

		return 0;
	}

	LRESULT PadConfigDialog::OnMouseMove(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		const int xPos = GET_X_LPARAM(lParam); 
		const int yPos = GET_Y_LPARAM(lParam); 
		// collision check
		for(int i = 0; i < m_gamepad.GetNumButtons(); i++) {
			ButtonInfo& button = m_gamepad.GetButton(i);
			RECT rect;
			rect.left = IMAGE_X + button.x;
			rect.top = IMAGE_Y + button.y;
			rect.right = IMAGE_X + button.x + button.width;
			rect.bottom = IMAGE_Y + button.y + button.height;

			if(rect.left <= xPos && xPos < rect.right &&
				rect.top <= yPos && yPos < rect.bottom) {
				if(m_mouseOverTarget != i) {
					InvalidateRect(&rect,TRUE);
					if(m_mouseOverTarget >= 0)
					{
						ButtonInfo& button = m_gamepad.GetButton(m_mouseOverTarget);
						RECT rect1;
						rect1.left = IMAGE_X + button.x;
						rect1.top = IMAGE_Y + button.y;
						rect1.right = IMAGE_X + button.x + button.width;
						rect1.bottom = IMAGE_Y + button.y + button.height;
						InvalidateRect(&rect1);
					}	
					m_mouseOverTarget = i;
				}
				return 0;
			}	
		}

		if(m_mouseOverTarget != -1) {
			ButtonInfo& button = m_gamepad.GetButton(m_mouseOverTarget);
			RECT rect1;
			rect1.left = IMAGE_X + button.x;
			rect1.top = IMAGE_Y + button.y;
			rect1.right = IMAGE_X + button.x + button.width;
			rect1.bottom = IMAGE_Y + button.y + button.height;
			InvalidateRect(&rect1);
		}
		m_mouseOverTarget = -1;
		return 0;
	}

	LRESULT PadConfigDialog::OnLButtonDown(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		if(m_mouseOverTarget != -1) {
			if(!m_configuration) {
				MessageBox(_T("To configure the keys, you must select or create a configuration."),_T("No configuration selected."),MB_OK);
			} else {
				ConfigurationKey pkey;
				ConfigurationValue pvalue;
	
				pkey.gamepadUUID = m_gamepadList[m_selectedGamepad].gamePad->GetUuid();
				pkey.buttonNo = m_mouseOverTarget;

				if(!m_configuration->GetValue(pkey,&pvalue)) {
					ButtonInfo& button = m_gamepad.GetButton(m_mouseOverTarget);
					pvalue.key[0] = button.defaultKey[0];
					pvalue.key[1] = button.defaultKey[1];
					pvalue.key[2] = button.defaultKey[2];
				}

				ButtonConfigDialog btnConfig(pvalue.key);
				if(btnConfig.DoModal() == 0) {
					pvalue.key[0] = btnConfig.key[0];
					pvalue.key[1] = btnConfig.key[1];
					pvalue.key[2] = btnConfig.key[2];

					m_configuration->SetValue(pkey,pvalue);

					RECT rect;
					rect.left = IMAGE_X;
					rect.top = IMAGE_Y;
					rect.right = rect.left + 480;
					rect.bottom = rect.top + 320;
					InvalidateRect(&rect);
				}
			}
		}
		return 0;
	}

	LRESULT PadConfigDialog::OnNewConfiguration(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		NewConfigurationDialog newConfigDlg;
		if(newConfigDlg.DoModal() == 0) {
			// check if configuration exists first
			FILE *fp = fopen((m_server.GetAppDataPath() + "/Configurations/" + newConfigDlg.configurationName + ".config").c_str(),"r");
			if(fp) {
				fclose(fp);
				MessageBox(_T("Configuration already exists."),_T("Could not create configuration."),MB_OK | MB_ICONERROR);
				return 0;
			}

			ConfigurationPtr config(new Configuration(newConfigDlg.configurationName));
			// copy configuration if checked
			if(newConfigDlg.copyCurrentConfiguration) {
				if(m_configuration) {
					*config = *m_configuration;
				}
			}

			try {
				m_server.SaveConfiguration(config,newConfigDlg.configurationName);
				// reload configuration
				ReloadConfigurationList();
			} catch(const std::runtime_error& e) {
				MessageBox(_T(e.what()),_T("Error creating configuration."),MB_OK | MB_ICONERROR);
			}

			RECT rect;
			rect.left = IMAGE_X;
			rect.top = IMAGE_Y;
			rect.right = rect.left + 480;
			rect.bottom = rect.top + 320;
			InvalidateRect(&rect);
		}
		return 0;
	}

	LRESULT PadConfigDialog::OnDeleteConfiguration(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		m_selectedConfiguration = m_cbConfiguration.GetCurSel() - 1;
		if(m_selectedConfiguration >= 0 && m_selectedConfiguration < (int)m_configurationList.size()) {
			if(MessageBox(_T("Are you sure you want to delete this configuration?"),_T("Delete configuration?"),MB_YESNO) == IDYES)
			{
				m_server.DeleteConfiguration(m_configurationList[m_selectedConfiguration]);
				ReloadConfigurationList();
				MessageBox(_T("Configuration was successfully deleted."),_T("Configuration delete"),MB_OK);
			}
		} else {
			MessageBox(_T("Can't delete default configuration."),_T("Nothing to delete."),MB_ICONWARNING | MB_OK);
		}
		return 0;
	}

	LRESULT PadConfigDialog::OnSaveConfiguration(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		m_selectedConfiguration = m_cbConfiguration.GetCurSel() - 1;
		try {
			if(m_selectedConfiguration < 0) 
				throw std::runtime_error("You cannot save over the default configuration.");
			if(m_selectedConfiguration >= (int)m_configurationList.size())
				throw std::runtime_error("Invalid configuration.");

			if(m_selectedConfiguration >= 0) {
				m_server.SaveConfiguration(m_configuration,m_configurationList[m_selectedConfiguration]);
				MessageBox(_T("Configuration was successfully saved."),_T("Configuration saved"),MB_OK);
			}

			// reload configurations for each device
			Server::DeviceList devices;
			m_server.GetDeviceList(&devices);
			for(Server::DeviceList::iterator device = devices.begin(); device != devices.end(); ++device)
			{
				ConfigurationPtr config = (*device)->GetConfiguration();
				if(config && config->GetName() == m_configurationList[m_selectedConfiguration]) {
					(*device)->SetConfiguration(m_server.LoadConfiguration(m_configurationList[m_selectedConfiguration]));
				}
			}
		} catch(const std::runtime_error& e) {
			MessageBox(_T(e.what()),_T("Error saving configuration"),MB_OK);
		}
		return 0;
	}

	LRESULT PadConfigDialog::OnChangeConfiguration(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		try {
			m_selectedConfiguration = m_cbConfiguration.GetCurSel() - 1;
			
			if(m_selectedConfiguration >= (int)m_configurationList.size()) {
				ReloadConfigurationList();
				throw std::runtime_error("Invalid configuration selected.");
			}

			if(m_selectedConfiguration >= 0) {
				m_configuration = m_server.LoadConfiguration(m_configurationList[m_selectedConfiguration]);
			} else {
				m_configuration.reset();
			}
			RECT rect;
			rect.left = IMAGE_X;
			rect.top = IMAGE_Y;
			rect.right = rect.left + 480;
			rect.bottom = rect.top + 320;
			InvalidateRect(&rect); 
		} catch(const std::runtime_error& e) {
			MessageBox(_T(e.what()),_T("Error loading configuration."),MB_OK | MB_ICONERROR);
		}

		return 0;
	}

	LRESULT PadConfigDialog::OnInstallGamepad(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		OPENFILENAME ofn;
		char filename[4096];

		ZeroMemory(&ofn,sizeof(ofn));
		ZeroMemory(filename,sizeof(filename));

		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = m_hWnd;
		ofn.lpstrFilter = "WifiPad Gamepads (*.gamepad)\0*.gamepad\0All files (*.*)\0*.*\0";
		ofn.lpstrTitle = "Choose a gamepad to install...";
		ofn.lpstrFile = filename;
		ofn.nMaxFile = 4096;
		ofn.Flags = OFN_FILEMUSTEXIST;
		if(GetOpenFileName(&ofn)) {
			try {
				std::string filebase = filename;
				size_t n = filebase.find_last_of('\\');
				if(n != std::string::npos) {
					filebase = filebase.substr(n + 1);
				} 
			
				// load gamepad to make sure it's valid
				GamePadPtr checkPad(new GamePad());
				try {
					FileDataSource source(filename);
					checkPad->LoadGamePad(source,0);
				} catch(const std::runtime_error& e) {
					throw std::runtime_error(std::string("Failed to install gamepad. The selected gamepad is invalid. Reason: ") + e.what());
				}
				
				// check for duplicate uuid
				try {
					m_server.GetGamePadPathByUuid(checkPad->GetUuid());
					MessageBox(_T("This gamepad is already installed. (Or matches a UUID of an existing gamepad.)"),_T("Error installing gamepad"),MB_ICONERROR);
					return 0;
				} catch(const std::runtime_error&) {
				}
				
				char curtime[32];
				sprintf(curtime,"%ld",(long)time(NULL));
				OS::CopyFile(filename,m_server.GetGamePadsPath() + "/" + std::string(curtime) + "." + filebase);
				
				MessageBox(_T("Gamepad installed!"),_T("Success!"),MB_ICONEXCLAMATION);
				
				m_server.LoadGamePads();
				ReloadGamePadList();
			} catch(const std::runtime_error& e) {
				MessageBox(_T(e.what()),_T("Error installing gamepad."),MB_OK | MB_ICONERROR);
			}
		}
		return 0;
	}

	LRESULT PadConfigDialog::OnDeleteGamepad(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		try {
			int selected = m_cbGamePad.GetCurSel();
			if(selected <= 0) throw std::runtime_error("You cannot delete the default gamepad.");
			
			if(MessageBox(_T(("Do you want to delete the gamepad \"" + m_gamepadList[selected].gamePad->GetName() + "\"?").c_str()),_T("Confirm deletion"),MB_YESNO) == IDYES) 
			{
				OS::DeleteFile(m_gamepadList[selected].path);
				m_server.LoadGamePads();
				ReloadGamePadList();
			}
		} catch(const std::runtime_error& e) {
			MessageBox(_T(e.what()),_T("Error deleting gamepad."),MB_OK | MB_ICONERROR);
		}
		return 0;
	}
}
