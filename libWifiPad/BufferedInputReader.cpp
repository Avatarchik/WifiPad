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
#include <string.h>
#include "DataSource.h"
#include "BufferedInputReader.h"

namespace WifiPad
{
	int BufferedInputReader::ReadBytes(uint8_t *data,int bytes)
	{
		if(m_validBytes > bytes) {
			memcpy(data,m_bufferPtr,bytes);
			m_bufferPtr += bytes;
			m_validBytes -= bytes;
			return bytes;
		}

		memcpy(data,m_bufferPtr,m_validBytes);
		data += m_validBytes;
		bytes -= m_validBytes;
		const int readBytes = m_validBytes;
		m_validBytes = 0;
		const int readMoreBytes = m_source.ReadBytes(data,bytes);
		if(readMoreBytes < 0) {
			if(readBytes == 0) return -1;
			return readBytes;
		}
		return readBytes + readMoreBytes;
	}

	// reads a line or up to maxBytes-1, always NULL terminates
	// and discards \r\n from the stored data
	int BufferedInputReader::ReadLine(uint8_t *data,int maxBytes)
	{
		int totalRead = 0;
		
		if(maxBytes <= 0) return 0;
		
		do {
			// locate \n in current buffer
			void *c = memchr(m_bufferPtr,'\n',m_validBytes);
			if(c) {
				int sizeToCopy = (uint8_t *)c - (uint8_t *)m_bufferPtr;
				if(sizeToCopy < maxBytes - 1) {
					memcpy(data,m_bufferPtr,sizeToCopy);
					data[sizeToCopy] = 0;// nul terminate
					m_bufferPtr += sizeToCopy + 1;
					m_validBytes -= sizeToCopy + 1;
					return totalRead + sizeToCopy + 1;
				}
			}
			
			if(m_validBytes >= maxBytes - 1) {
				memcpy(data,m_bufferPtr,maxBytes - 1);
				data[maxBytes - 1] = 0;// nul terminate
				m_bufferPtr += maxBytes - 1;
				m_validBytes -= maxBytes - 1;
				return totalRead + maxBytes - 1;
			}
			
			// reload buffer
			memcpy(data,m_bufferPtr,m_validBytes);
			totalRead += m_validBytes;
			data += m_validBytes;
			maxBytes -= m_validBytes;
			m_validBytes =  m_source.ReadBytes(&m_buffer[0],m_bufferSize);
			if(m_validBytes < 0) {
				m_validBytes = 0;
				return totalRead ? totalRead : -1;
			}
			m_bufferPtr = &m_buffer[0];
		} while(maxBytes >= 0);
		
		return totalRead;
	}

	// skip bytes
	int BufferedInputReader::SkipBytes(int bytes)
	{
		if(m_validBytes > bytes) {
			m_bufferPtr += bytes;
			m_validBytes -= bytes;
			return bytes;
		}

		bytes -= m_validBytes;
		m_validBytes = 0;

		return m_source.SkipBytes(bytes);
	}
}
