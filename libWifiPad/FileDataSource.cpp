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
#include <stdio.h>
#include <string>
#include <stdexcept>
#include "FileDataSource.h"

namespace WifiPad
{
	FileDataSource::FileDataSource(const std::string& filename) : DataSource(filename)
	{
		m_fp = fopen(filename.c_str(),"rb");
		if(!m_fp) throw std::runtime_error(std::string("Could not open file ") + filename + ".\n");
	}

	FileDataSource::~FileDataSource() 
	{ 
		fclose(m_fp);
	}

	int FileDataSource::ReadBytes(uint8_t *data,int size) 
	{
		int read_bytes = fread(data,1,size,m_fp);
		return read_bytes <= 0 ? -1 : read_bytes;
	}

	int FileDataSource::SkipBytes(int bytes)
	{ 
		off_t pos = ftell(m_fp);
		fseek(m_fp,bytes,SEEK_CUR); 
		return ftell(m_fp) - pos;
	}
}