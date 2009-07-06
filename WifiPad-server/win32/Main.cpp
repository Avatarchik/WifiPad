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
#include "Protocol.h"
#include "resource.h"
#include "Server.h"
#include "WifiPadWindow.h"

#include "Socket.h"
#include "Protocol.h"

CComModule _Module;

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR cmdLine,int nShow)
{
	using namespace WifiPad;

	_Module.Init(NULL,hInstance);

	MSG msg;
	try {
		char hostname[256];
		gethostname(hostname,255);
		hostname[255] = 0;
		Server server("0.0.0.0",8989);
		WifiPadWindow mainWindow(server);
		
		if(!mainWindow.Create(NULL,CWindow::rcDefault,_T("WifiPad"))) {
			MessageBox(NULL,"Failed to create window!","Error",MB_ICONEXCLAMATION | MB_OK);
			return -1;
		}

		mainWindow.InitSysTray();
		mainWindow.ShowWindow(SW_HIDE);
		mainWindow.UpdateWindow();

		server.Start();

		while(GetMessage(&msg,NULL,0,0) > 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		_Module.Term();
	} catch(const std::exception& e) {
		::MessageBox(NULL,e.what(),"Unhandled exception",MB_ICONWARNING | MB_OK);
		return -1;
	}
	
	return msg.wParam;
}
