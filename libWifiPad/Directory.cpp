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
#if _WIN32
#include <windows.h>
#else
#include <dirent.h>
#endif
#include <stdexcept>
#include <string>
#include "Directory.h"

namespace WifiPad
{
#if _WIN32
	Directory::Directory(const std::string& path) 
	{
		WIN32_FIND_DATA findData;
		m_dir = ::FindFirstFile(path.c_str(),&findData);
		if(m_dir == INVALID_HANDLE_VALUE) m_dir = NULL;
		else this->path = findData.cFileName;
	}

	Directory& Directory::operator ++()
	{
		WIN32_FIND_DATA findData;
		if(!::FindNextFile(m_dir,&findData)) {
			::FindClose(m_dir);
			m_dir = NULL;
		} else {
			this->path = findData.cFileName;
		}
		return *this;
	}
#else
	Directory::Directory(const std::string& path) 
	{
		m_dir = opendir(path.c_str());
		if(m_dir) {
			++(*this);
		}
	}
	
	Directory& Directory::operator ++()
	{
		dirent *ent = readdir(m_dir);
		if(ent) {
			this->path = ent->d_name;
		} else {
			closedir(m_dir);
			m_dir = NULL;
		}
		return *this;
	}
#endif
}
