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
#ifndef __ButtonConfigDialog_h
#define __ButtonConfigDialog_h

#include "stdafx.h"
#include <rpc.h>
#include "resource.h"

namespace WifiPad
{
	class Server;
	class ButtonConfigDialog : public CDialogImpl<ButtonConfigDialog>
	{
	private:
		LONG m_oldWinProc;
		CComboBox m_eKey1, m_eKey2, m_eKey3;
	public:
		int key[3];

		ButtonConfigDialog(int key[3]) { 
			this->key[0] = key[0];
			this->key[1] = key[1]; 
			this->key[2] = key[2]; 
		}

		enum { IDD = IDD_BUTTON_CONFIG_DIALOG };

		BEGIN_MSG_MAP(PadConfigDialog)
			MESSAGE_HANDLER(WM_INITDIALOG,OnInitDialog)
			COMMAND_ID_HANDLER(IDC_OK,OnOK)
			COMMAND_ID_HANDLER(IDC_CANCEL,OnCancel)
			COMMAND_ID_HANDLER(IDC_CLEAR,OnClear)
		END_MSG_MAP()

		LRESULT OnInitDialog(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled);

		LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
		{
			EndDialog(-1);
			return 0;
		}

		LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

		LRESULT OnClear(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
		{
			m_eKey1.SetWindowText("");
			m_eKey2.SetWindowText("");
			m_eKey3.SetWindowText("");
			return 0;
		}
	};
}

#endif
