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
#ifndef __PadConfigDialog_h
#define __PadConfigDialog_h

#include "stdafx.h"
#include <stdio.h>
#include "resource.h"
#include "GamePad.h"
#include "NewConfigurationDialog.h"
#include "Server.h"

namespace WifiPad
{
	class PadConfigDialog : public CDialogImpl<PadConfigDialog>
	{
	private:
		Server& m_server;
		int m_mouseOverTarget;
		GamePad m_gamepad;
		ConfigurationPtr m_configuration;
		int m_selectedGamepad;
		int m_selectedConfiguration;

		Server::ConfigurationList m_configurationList;
		Server::GamePadList m_gamepadList;

		// window controls
		CEdit m_eServerName;
		CComboBox m_cbConfiguration;
		CComboBox m_cbGamePad;
		CButton m_bNewConfiguration;
		CButton m_bDeleteConfiguration;
		CButton m_bSaveConfiguration;
		CButton m_bShowKeys;
		CButton m_bShowTapZones;
		CTrackBarCtrl m_tbMouseSpeed;

		enum {
			IMAGE_X = 6,
			IMAGE_Y = 120
		};

		void ReloadConfigurationList();
		void ReloadGamePadList();
	public:
		PadConfigDialog(Server& server) : m_server(server), m_mouseOverTarget(-1),
			m_selectedGamepad(0), m_selectedConfiguration(-1)
		{
		}

		enum { IDD = IDD_PAD_CONFIG_DIALOG };

		BEGIN_MSG_MAP(PadConfigDialog)
			MESSAGE_HANDLER(WM_INITDIALOG,OnInitDialog)
			MESSAGE_HANDLER(WM_CLOSE,OnClose)
			MESSAGE_HANDLER(WM_PAINT,OnPaint)
			MESSAGE_HANDLER(WM_MOUSEMOVE,OnMouseMove)
			MESSAGE_HANDLER(WM_LBUTTONDOWN,OnLButtonDown)
			MESSAGE_HANDLER(WM_HSCROLL,OnChangeMouseSpeed)
			COMMAND_ID_HANDLER(IDM_ADD_REMOVE_GAMEPAD,OnAddRemoveGamePad)
			COMMAND_ID_HANDLER(IDM_ABOUT_WIFIPAD,OnAboutWifiPad)
			COMMAND_ID_HANDLER(IDM_EXIT,OnExit);
			COMMAND_ID_HANDLER(IDC_NEW_CONFIGURATION,OnNewConfiguration);
			COMMAND_ID_HANDLER(IDC_DELETE_CONFIGURATION,OnDeleteConfiguration);
			COMMAND_ID_HANDLER(IDC_SAVE_CONFIGURATION,OnSaveConfiguration);
			COMMAND_ID_HANDLER(IDC_SHOW_KEYS,OnShowKeys)
			COMMAND_ID_HANDLER(IDC_SHOW_TAP_ZONES,OnShowTapZones)
			COMMAND_HANDLER(IDC_SERVER_NAME,EN_CHANGE,OnChangeServerName);
			COMMAND_HANDLER(IDC_CONFIGURATION,CBN_SELCHANGE,OnChangeConfiguration)
			COMMAND_HANDLER(IDC_GAMEPAD,CBN_SELCHANGE,OnChangeGamePad)
			COMMAND_ID_HANDLER(IDC_INSTALL_GAMEPAD,OnInstallGamepad);
			COMMAND_ID_HANDLER(IDC_DELETE_GAMEPAD,OnDeleteGamepad);
		END_MSG_MAP()


		LRESULT OnClose(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
		{
			m_gamepad.Clear();
			EndDialog(0);
			return 0;
		}

		LRESULT OnExit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
		{
			PostQuitMessage(0);
			return 0;
		}

		LRESULT OnAboutWifiPad(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
		{
			::ShellExecute(NULL,"open","http://www.wifipad.com/",NULL,NULL,SW_SHOW);
			return 0;
		}

		LRESULT OnAddRemoveGamePad(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
		{
			::ShellExecute(NULL,"explore",(m_server.GetAppDataPath() + "\\Gamepads").c_str(),NULL,NULL,SW_SHOW);
			return 0;
		}

		LRESULT OnShowTapZones(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
		{
			InvalidateRect(NULL);
			return 0;
		}

		LRESULT OnShowKeys(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
		{
			InvalidateRect(NULL);
			return 0;
		}

		LRESULT OnChangeServerName(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
		{
			char name[128];
			::ZeroMemory(name,128);
			m_eServerName.GetLine(0,name,127);
			m_server.SetServerName(name);
			return 0;
		}

		LRESULT OnNewConfiguration(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnDeleteConfiguration(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnSaveConfiguration(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnChangeConfiguration(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

		LRESULT OnInitDialog(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
		LRESULT OnPaint(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
		LRESULT OnMouseMove(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
		LRESULT OnLButtonDown(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);

		LRESULT OnChangeGamePad(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnChangeMouseSpeed(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);

		LRESULT OnInstallGamepad(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
		LRESULT OnDeleteGamepad(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	};
}

#endif
