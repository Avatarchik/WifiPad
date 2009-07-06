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
#include "Socket.h"
#include "DataSource.h"
#include "SocketDataSource.h"

namespace WifiPad
{
	int SocketDataSource::ReadBytes(uint8_t *data,int size)
	{
		return m_socket.ReadFully(data,size);
	}

	int SocketDataSource::SkipBytes(int size)
	{
		char buffer[1024];
		int skipped = 0;
		int readBytes;
		while(size > 0) {
			readBytes = m_socket.ReadFully(buffer,size > 1024 ? 1024 : size);
			if(readBytes <= 0) break;
			skipped += readBytes;
			size -= readBytes;
		}
		return skipped;
	}
}
