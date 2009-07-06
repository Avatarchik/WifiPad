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
#ifndef __DataSink_h
#define __DataSink_h

#include <string>

namespace WifiPad
{
	class DataSink
	{
	public:
		std::string name;
	
		DataSink(const std::string& name) : name(name) {}
		
		virtual ~DataSink() {}

		// writes up to size bytes of data to the sink
		// returns number of bytes written, or -1 on error
		virtual int WriteBytes(const uint8_t *data,int size) = 0;
	};
}

#endif
