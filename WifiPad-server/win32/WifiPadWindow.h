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
#ifndef __WifiPadWindow_h
#define __WifiPadWindow_h

#include "stdafx.h"
#include "resource.h"
#include "Server.h"
#include "PadConfigDialog.h"

#define WM_SYSTRAY (WM_USER+1)

namespace WifiPad
{
	typedef CWinTraits<WS_OVERLAPPEDWINDOW> WifiPadWindowTraits;
	class WifiPadWindow : public CWindowImpl<WifiPadWindow,CWindow,WifiPadWindowTraits>
	{
	private:
		NOTIFYICONDATA m_nid;
		Server& m_server;
		PadConfigDialog m_padConfigDialog;
	public:
		WifiPadWindow(Server& server);
		~WifiPadWindow();

		void InitSysTray();

		DECLARE_WND_CLASS(_T("WifiPad"))

		BEGIN_MSG_MAP(WifiPadWindow)
			MESSAGE_HANDLER(WM_SYSTRAY,OnSysTray)
			MESSAGE_HANDLER(WM_CLOSE,OnClose)
			MESSAGE_HANDLER(WM_DESTROY,OnDestroy)
			COMMAND_ID_HANDLER(IDM_SHOW_CONFIG,OnMenuShowConfig)
			COMMAND_ID_HANDLER(IDM_EXIT,OnMenuExit)
		END_MSG_MAP()

		LRESULT OnSysTray(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
		LRESULT OnClose(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);
		LRESULT OnDestroy(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);

		LRESULT OnMenuExit(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled)
		{
			PostQuitMessage(0);
			return 0;
		}

		LRESULT OnMenuShowConfig(WORD wNotifyCode,WORD wID,HWND hWndCtl,BOOL& bHandled)
		{
			if(m_padConfigDialog.m_hWnd == NULL)
				m_padConfigDialog.DoModal();
			return 0;
		}

	};
}

#endif
