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
#ifndef __BufferedInputReader_h
#define __BufferedInputReader_h

#include <vector>
#include <inttypes.h>
#include "DataSource.h"

namespace WifiPad
{
	class DataSource;

	class BufferedInputReader : public DataSource
	{
	private:
		DataSource& m_source;
		int m_bufferSize;
		std::vector<uint8_t> m_buffer;
		uint8_t *m_bufferPtr;
		int m_validBytes;
	public:
		BufferedInputReader(DataSource& source,int bufferSize = 4096)
			: DataSource(source.name), m_source(source), m_bufferSize(bufferSize), m_buffer(bufferSize),
			m_bufferPtr(NULL), m_validBytes(0) { }

		// reads bytes from stream
		int ReadBytes(uint8_t *data,int bytes);

		// reads a line or up to maxBytes-1, always NULL terminates
		// and discards \r\n from the stored data
		int ReadLine(uint8_t *data,int maxBytes);

		// skip bytes
		int SkipBytes(int bytes);
	};
}

#endif
