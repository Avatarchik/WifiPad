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
#ifndef __UUID_h
#define __UUID_h

#include <string>
#include <string.h>
#include <stdexcept>
#if _WIN32
#include <rpc.h>
#else
#include <uuid/uuid.h>
#endif

namespace WifiPad
{
	class Uuid
	{
	private:
#if _WIN32
		::UUID m_uuid;
#else
		uuid_t m_uuid;
#endif
	public:
		void Generate()
		{
#if _WIN32
			::UuidCreate(&m_uuid);
#else
			uuid_generate(m_uuid);
#endif
		}

		void ParseString(const std::string& uuidStr)
		{
#if _WIN32
			if(::UuidFromString((RPC_CSTR)uuidStr.c_str(),&m_uuid)== RPC_S_INVALID_STRING_UUID)
			{
				throw std::runtime_error("Invalid UUID\n");
			}

#else
			if(uuid_parse(uuidStr.c_str(),m_uuid) == -1)
			{
				throw std::runtime_error("Invalid UUID\n");
			}
#endif
		}

		void UnparseString(char outUuidStr[37]) const
		{
#if _WIN32
			unsigned char *str;
			::UuidToString(&m_uuid,&str);
			strncpy(outUuidStr,(char *)str,36);
			outUuidStr[36] = 0;
			::RpcStringFree(&str);
#else
			uuid_unparse(m_uuid,outUuidStr);
			outUuidStr[36] = 0;
#endif
		}

		bool operator ==(const Uuid& rhs) const
		{
			return memcmp(this,&rhs,sizeof(Uuid)) == 0;
		}
	};
}

#endif
