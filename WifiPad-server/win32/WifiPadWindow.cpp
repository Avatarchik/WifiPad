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
#include "resource.h"
#include "Server.h"
#include "PadConfigDialog.h"
#include "WifiPadWindow.h"

namespace WifiPad
{
	WifiPadWindow::WifiPadWindow(Server& server) : m_server(server), m_padConfigDialog(server)
	{
		::ZeroMemory(&m_nid,sizeof(NOTIFYICONDATA));
	}

	WifiPadWindow::~WifiPadWindow()
	{
		DestroyWindow();
		::Shell_NotifyIcon(NIM_DELETE,&m_nid);
	}

	void WifiPadWindow::InitSysTray()
	{
		::ZeroMemory(&m_nid,sizeof(NOTIFYICONDATA));
		m_nid.cbSize = sizeof(NOTIFYICONDATA);
		m_nid.hWnd = m_hWnd;
		m_nid.uID = 3533;
		m_nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
		m_nid.hIcon = (HICON)LoadImage(
			::GetModuleHandle(NULL),
			(LPCTSTR)MAKEINTRESOURCE(IDI_WIFIPAD_ICON),
			(UINT)IMAGE_ICON,
			16,
			16,
			0);
		m_nid.uCallbackMessage = WM_SYSTRAY;
		strncpy(m_nid.szTip,_T("WifiPad"),64);
		m_nid.szTip[63] = 0;

		::Shell_NotifyIcon(NIM_ADD,&m_nid);
	}

	LRESULT WifiPadWindow::OnSysTray(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		switch(LOWORD(lParam)) {
			case WM_LBUTTONDOWN:
			{
				if(m_padConfigDialog.m_hWnd == NULL)
					m_padConfigDialog.DoModal();
				break;
			}
			case WM_RBUTTONDOWN:
			{
				CMenu menu;
				menu.LoadMenu(IDR_CONTEXT_MENU);
				CMenuHandle subMenu = menu.GetSubMenu(0);
				if(subMenu) {
					POINT pt;
					GetCursorPos(&pt);
					subMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,pt.x,pt.y,m_hWnd);
				}
			}
		}
		return 0;
	}

	LRESULT WifiPadWindow::OnClose(UINT uMsg,WPARAM wParsam,LPARAM lParam,BOOL& bHandled)
	{
		PostQuitMessage(0);
		return 0;
	}

	LRESULT WifiPadWindow::OnDestroy(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		return 0;
	}
}
