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
#ifndef __Ar_h
#define __Ar_h

#include <stdlib.h>
#include <string.h>
#include "DataSource.h"

namespace WifiPad
{
	struct ArFileHeader
	{
		char filename[16]; // ASCII 15-char
		char timestamp[12+1]; // decimal
		char ownerId[6+1]; // decimal
		char groupId[6+1]; // decimal
		char fileMode[8+1]; // octal
		char fileSize[10+1]; // decimal
		char magic[2]; // \140\012

		bool ReadHeader(DataSource& source) 
		{
			if(source.ReadBytes((uint8_t *)this->filename,16) == -1) return false;
			this->filename[15] = 0;

			// gnu ar format
			char *p;
			if((p = strrchr(this->filename,'/'))) *p = 0;
			
			// trim
			int len = strlen(this->filename);
			if(len) {
				p = this->filename + len - 1;
				while(len-- && isspace(*p)) {
					*p = 0;
					p--;
				}
			}

			if(source.ReadBytes((uint8_t *)this->timestamp,12) == -1) return false;
			this->timestamp[12] = 0;
			if(source.ReadBytes((uint8_t *)this->ownerId,6) == -1) return false;
			this->ownerId[6] = 0;
			if(source.ReadBytes((uint8_t *)this->groupId,6) == -1) return false;
			this->groupId[6] = 0;
			if(source.ReadBytes((uint8_t *)this->fileMode,8) == -1) return false;
			this->fileMode[8] = 0;
			if(source.ReadBytes((uint8_t *)this->fileSize,10) == -1) return false;
			this->fileSize[10] = 0;
			if(source.ReadBytes((uint8_t *)this->magic,2) == -1) return false;
			return true;
		}

		const char *GetFilename() const { return this->filename; }
		int GetTimestamp() const { return strtol(this->timestamp,NULL,10); }
		int GetOwnerID() const { return strtol(this->ownerId,NULL,10); }
		int GetGroupID() const { return strtol(this->groupId,NULL,10); }
		int GetFileMode() const { return strtol(this->fileMode,NULL,8); }
		int GetFileSize() const { return strtol(this->fileSize,NULL,10); }
		bool VerifyMagic() const { return this->magic[0] == 96 && this->magic[1] == 10; }
	};
}

#endif
