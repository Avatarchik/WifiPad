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
#ifndef __OS_h
#define __OS_h

#include <string>

namespace WifiPad
{
	namespace OS
	{
		std::string GetPathToExe();
		std::string GetResourcesPath();
		std::string GetAppDataPath();
		void CreateDirectory(const std::string& path,int mode);
		void DeleteDirectory(const std::string& path);
		void DeleteFile(const std::string& path);
		void CopyFile(const std::string& srcPath,const std::string& destPath);
		bool FileExists(const std::string& path);
	}
}

#endif
