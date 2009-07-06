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
#include <inttypes.h>
#include <string>
#include <stdexcept>
#include "FileDataSink.h"

namespace WifiPad
{
	FileDataSink::FileDataSink(const std::string& filename) : DataSink(filename)
	{
		m_fp = fopen(filename.c_str(),"wb");
		if(!m_fp) throw std::runtime_error(std::string("Could not open file ") + filename + ".\n");
	}

	FileDataSink::~FileDataSink() 
	{ 
		fclose(m_fp);
	}

	int FileDataSink::WriteBytes(const uint8_t *data,int size) 
	{
		int writtenBytes = fwrite(data,1,size,m_fp);
		return writtenBytes <= 0 ? -1 : writtenBytes;
	}

}