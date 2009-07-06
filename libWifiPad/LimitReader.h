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
#ifndef __LimitReader_h
#define __LimitReader_h

#include "DataSource.h"

namespace WifiPad
{
	class LimitReader : public DataSource
	{
	private:
		DataSource& m_source;
		int m_limit;
	public:
		LimitReader(DataSource& source,int limit) : DataSource(source.name), m_source(source), m_limit(limit) {}
		virtual ~LimitReader();
		
		virtual int ReadBytes(uint8_t *data,int size);
		
		virtual int SkipBytes(int size);
	};
}

#endif
