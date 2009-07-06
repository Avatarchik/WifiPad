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
#include "resource.h"
#include <sstream>
#include <windows.h>
#include "KeySyms.h"
#include "ButtonConfigDialog.h"

namespace WifiPad
{
	static LRESULT CALLBACK ButtonConfigFieldSubClass(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
	{
		// trap keyboard
		switch(uMsg) {
			case WM_GETDLGCODE:
				// specify to handle return/tab/escape keys, etc...
               return (DLGC_WANTALLKEYS | CallWindowProc((WNDPROC)GetWindowLongPtr(hWnd,GWLP_USERDATA),hWnd,uMsg,wParam,lParam));
			case WM_SYSKEYDOWN:
			case WM_KEYDOWN:
			{
				// handle keydowns and set the box to the keyname
				::SendMessage(hWnd,WM_SETTEXT,NULL,(LPARAM)KeySyms::MapKey(wParam).c_str());
				return 0;
			}
			case WM_KEYUP:
			case WM_CHAR:
				return 0;
		}

		LONG_PTR wndProc = GetWindowLongPtr(hWnd,GWLP_USERDATA);
		return CallWindowProc((WNDPROC)wndProc,hWnd,uMsg,wParam,lParam);
	}

	LRESULT ButtonConfigDialog::OnInitDialog(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		// subclass the button config fields
		m_eKey1 = GetDlgItem(IDC_BUTTON1);
		m_eKey2 = GetDlgItem(IDC_BUTTON2);
		m_eKey3 = GetDlgItem(IDC_BUTTON3);

		// populate combo boxes with special buttons
		CComboBox *c[] = { &m_eKey1, &m_eKey2, &m_eKey3, NULL };
		for(int i = 0; c[i]; ++i) {
			c[i]->AddString("MOUSEUP");
			c[i]->AddString("MOUSEDOWN");
			c[i]->AddString("MOUSELEFT");
			c[i]->AddString("MOUSERIGHT");
			c[i]->AddString("WHEELUP");
			c[i]->AddString("WHEELDOWN");
			c[i]->AddString("LBUTTON");
			c[i]->AddString("RBUTTON");
			c[i]->AddString("MBUTTON");

			// sub class the edit windows
			HWND editWnd = ::FindWindowEx(*c[i],NULL,"EDIT",NULL);
			if(editWnd) {
				c[i]->SetWindowText(KeySyms::MapKey(this->key[i]).c_str());
				LONG_PTR wndProc = ::GetWindowLongPtr(editWnd,GWLP_WNDPROC);
				::SetWindowLongPtr(editWnd,GWLP_USERDATA,(LONG_PTR)wndProc); // store old proc into userdata
				::SetWindowLongPtr(editWnd,GWLP_WNDPROC,(LONG_PTR)ButtonConfigFieldSubClass);
			}
		}

		return 0;
	}

	LRESULT ButtonConfigDialog::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		char string[32];
		int n;

		::ZeroMemory(string,32);
		n = m_eKey1.GetWindowText(string,31);
		string[n] = 0;
		this->key[0] = KeySyms::MapSymbol(string);
		
		::ZeroMemory(string,32);
		n = m_eKey2.GetWindowText(string,31);
		string[n] = 0;
		this->key[1] = KeySyms::MapSymbol(string);
		
		::ZeroMemory(string,32);
		n = m_eKey3.GetWindowText(string,31);
		string[n] = 0;
		this->key[2] = KeySyms::MapSymbol(string);
		
		EndDialog(0);
		return 0;
	}
}
