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
#ifndef __FileDataSink_h
#define __FileDataSink_h

#include <stdio.h>
#include <string>
#include <inttypes.h>
#include "DataSink.h"

namespace WifiPad
{
	class FileDataSink : public DataSink
	{
	private:
		FILE *m_fp;
	public:
		FileDataSink(const std::string& filename);
		virtual ~FileDataSink();

		virtual int WriteBytes(const uint8_t *data,int size);
	};
}

#endif
