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
#ifndef __NewConfigurationDialog_h
#define __NewConfigurationDialog_h

#include "stdafx.h"
#include <ctype.h>
#include "resource.h"

namespace WifiPad
{
	class NewConfigurationDialog : public CDialogImpl<NewConfigurationDialog>
	{
	private:
		CEdit m_eConfigurationName;
		CButton m_bCopyCurrentConfiguration;
	public:
		std::string configurationName;
		bool copyCurrentConfiguration;

		enum { IDD = IDD_NEW_CONFIGURATION_DIALOG };

		BEGIN_MSG_MAP(PadConfigDialog)
			MESSAGE_HANDLER(WM_INITDIALOG,OnInitDialog)
			COMMAND_ID_HANDLER(IDOK,OnOK)
			COMMAND_ID_HANDLER(IDCANCEL,OnCancel)
		END_MSG_MAP()

		LRESULT OnInitDialog(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
		{
			m_eConfigurationName = GetDlgItem(IDC_CONFIGURATION_NAME);
			m_bCopyCurrentConfiguration = GetDlgItem(IDC_COPY_CURRENT_CONFIGURATION);
			m_bCopyCurrentConfiguration.SetCheck(BST_CHECKED);
			m_eConfigurationName.SetFocus();
			return 0;
		}

		LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
		{
			EndDialog(-1);
			return 0;
		}

		LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	};
}

#endif
