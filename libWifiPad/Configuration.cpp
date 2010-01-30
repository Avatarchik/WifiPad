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
#include <sstream>
#include <string>
#include <string.h>
#include <inttypes.h>
#include <stdexcept>
#include "DataSource.h"
#include "DataSink.h"
#include "BufferedInputReader.h"
#include "Configuration.h"

#if _WIN32
extern char *strsep (char **stringp, const char *delim);
#endif

namespace WifiPad
{
	void Configuration::Load(const std::string& name, DataSource& source)
	{
		ScopedLock lock(m_loadLock);
		BufferedInputReader reader(source);
		char line[4096];
		ConfigurationKey key;
		ConfigurationValue value;

		Clear();
 
		m_name = name;

		char *linep;
		// format is relly easy..
		// gamepad-uuid:buttonNo:buttonKeys1,buttonKeys2,buttonKeys3
		while(reader.ReadLine((uint8_t *)line,4096) != -1) {
			const char *gamepadUUID = strsep(&linep,":");
			const char *buttonNo = strsep(&linep,":");
			char *buttonKeys = strsep(&linep,":");

			if(!gamepadUUID || !buttonNo || !buttonKeys)
				continue;
			try {
				key.gamepadUUID.ParseString(gamepadUUID);
			}
			catch(std::runtime_error&)
			{	
				continue;
			}
			key.buttonNo = strtol(buttonNo,NULL,10);
			
			linep = buttonKeys;
			char *button = strsep(&linep,",");
			value.key[0] = button ? strtol(button,NULL,0) : -1;
			button = strsep(&linep,",");
			value.key[1] = button ? strtol(button,NULL,0) : -1;
			button = strsep(&linep,",");
			value.key[2] = button ? strtol(button,NULL,0) : -1;

			SetValue(key,value);
		}

		m_changed = false;
	}

	void Configuration::Save(DataSink& sink)
	{
		ScopedLock lock(m_lock);
		ScopedLock lock2(m_loadLock);
		KeyHash::const_iterator it;
		std::stringstream line;

		for(it = m_map.begin(); it != m_map.end(); ++it)
		{
			char gamepadUUID[37];
			it->first.gamepadUUID.UnparseString(gamepadUUID);
			line.str("");
			line <<
				gamepadUUID << ":" <<
				it->first.buttonNo << ":" <<
				it->second.key[0] << "," <<
				it->second.key[1] << "," <<
				it->second.key[2] << "\n";

			sink.WriteBytes((uint8_t *)line.str().c_str(),line.str().size());
		}
	}
}
