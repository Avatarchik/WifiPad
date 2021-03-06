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
#ifndef __Directory_h
#define __Directory_h

#include <stdexcept>
#include <string>
#if _WIN32
#include <windows.h>
#else
#include <dirent.h>
#endif

namespace WifiPad
{
	class Directory
	{
	private:
#if _WIN32
		HANDLE m_dir;
#else
		DIR *m_dir;
#endif
	public:
		std::string path;

		Directory(const std::string& path);
		~Directory() { 
#if _WIN32
			if(m_dir) FindClose(m_dir); 
#else
			if(m_dir) closedir(m_dir);
#endif
		}

		Directory& operator ++();

		Directory operator ++(int)
		{
			Directory old(*this);
			++(*this);
			return old;
		}

		operator bool() const { return m_dir != NULL; }
	};
}

#endif
