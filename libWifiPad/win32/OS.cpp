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
#include <windows.h>
#include <stdexcept>
#include <string>
#include "OS.h"

namespace WifiPad
{
	namespace OS {
		std::string GetPathToExe()
		{
			char buffer[4096];
			if(::GetModuleFileName(NULL,buffer,4096) == 4096)
				throw std::runtime_error("Could not determine path to executable.");
			
			char *p = buffer;
			while(*p) {
				if(*p == '\\') *p = '/';
				p++;
			}
			
			return buffer;
		}
		
		std::string GetResourcesPath()
		{
			// on windows it is just the same folder as exe
			std::string pathToExe = OS::GetPathToExe();
			size_t pos = pathToExe.find_last_of('/');
			if(pos != std::string::npos) {
				return pathToExe.substr(0,pos);
			}
			return "";
		}
		
		std::string GetAppDataPath()
		{
			char *appPath = getenv("APPDATA");
			if(appPath == NULL) {
				throw std::runtime_error("Cannot find Application folder.");
			}
			std::string appDataPath = appPath;
			appDataPath += "/WifiPad";
			return appDataPath;
		}
		
		void CreateDirectory(const std::string& path,int mode)
		{
			::CreateDirectory(path.c_str(),NULL);
		}
		
		void DeleteDirectory(const std::string& path)
		{
			::RemoveDirectory(path.c_str());
		}
		
		void DeleteFile(const std::string& path)
		{
			::DeleteFile(path.c_str());
		}
		
		void CopyFile(const std::string& srcPath,const std::string& destPath)
		{
			::CopyFile(srcPath.c_str(),destPath.c_str(),TRUE);
		}

		bool FileExists(const std::string& path)
		{
			FILE *fp = fopen(path.c_str(),"r");
			if(!fp) return false;
			fclose(fp);
			return true;
		}
	}
}
