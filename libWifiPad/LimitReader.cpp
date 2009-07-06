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

#include "LimitReader.h"

namespace WifiPad
{
	LimitReader::~LimitReader() 
	{
		SkipBytes(m_limit);
	}
	
	int LimitReader::ReadBytes(uint8_t *data,int size) 
	{
		if(!m_limit) return -1;
		int readBytes = m_source.ReadBytes(data,size > m_limit ? m_limit : size);
		if(readBytes > 0) m_limit -= readBytes;
		return readBytes;
	}
	
	int LimitReader::SkipBytes(int size) 
	{
		if(!m_limit) return 0;
		int skippedBytes = m_source.SkipBytes(size > m_limit ? m_limit : size);
		m_limit -= skippedBytes;
		return skippedBytes;
	}
}