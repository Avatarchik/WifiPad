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
#include <ctype.h>
#include "NewConfigurationDialog.h"

namespace WifiPad
{
	LRESULT NewConfigurationDialog::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		char name[64];
		::ZeroMemory(name,64);

		// check format of string
		const int n = m_eConfigurationName.GetLine(0,name,32);
		if(n >= 32) MessageBox(_T("Configuration name must be < 32 characters in length."),_T("Invalid configuration name."),MB_ICONERROR | MB_OK);
		else if(n == 0) MessageBox(_T("Configuration name is empty."),_T("Invalid Configuration name."),MB_ICONERROR | MB_OK);
		else {
			name[n] = 0;
			// check for weird characters
			char *c = name;
			while(*c) {
				if(!isprint(*c) ||
					*c == '/' || *c == '\\' || *c == ':' ||
					*c == '*' || *c == '?' || *c == '"' ||
					*c == '<' || *c == '>' || *c == '|') {
						MessageBox(_T("Configuration name cannot contain the follow characters: / \\ : * ? \" < > |"),_T("Invalid configuration name."),MB_ICONERROR | MB_OK);
						return 0;
				}
				++c;
			}
			this->configurationName = name;
			this->copyCurrentConfiguration = m_bCopyCurrentConfiguration.GetCheck() == BST_CHECKED;
			EndDialog(0);
		}
		
		return 0;
	}
}
