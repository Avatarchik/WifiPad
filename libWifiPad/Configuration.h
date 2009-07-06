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
#ifndef __Configuration_h
#define __Configuration_h

#include <boost/unordered_map.hpp>
#include <tr1/memory>
#include <string>
#include "Mutex.h"
#include "Uuid.h"

namespace WifiPad
{
	struct ConfigurationKey
	{
		Uuid gamepadUUID;
		int buttonNo;

		bool operator ==(const ConfigurationKey& rhs) const 
		{
			return this->gamepadUUID == rhs.gamepadUUID 
				&& this->buttonNo == rhs.buttonNo;
		}
	};

	struct ConfigurationValue
	{
		int key[3];
	};

	struct ConfigurationHash : std::unary_function<ConfigurationKey,std::size_t>
	{
		std::size_t operator()(ConfigurationKey const& x) const
		{
			return boost::hash_range((unsigned char *)&x,(unsigned char *)&x + sizeof(ConfigurationKey));
		}
	};

	class DataSource;
	class DataSink;

	class Configuration
	{
	private:
		typedef boost::unordered_map<ConfigurationKey,ConfigurationValue,ConfigurationHash> KeyHash;
		KeyHash m_map;
		std::string m_name;
		bool m_changed;
		mutable Mutex m_lock, m_loadLock;
	public:
		Configuration(const std::string& name = "Custom") : m_name(name), m_changed(false) { }
		Configuration(const std::string& name, DataSource& source) : m_name(name), m_changed(false) { Load(name,source); }

		std::string GetName() const { ScopedLock lock(m_lock); return m_name; }
		bool IsChanged() const { ScopedLock lock(m_lock); return m_changed; }

		void Load(const std::string& name, DataSource& source);
		void Save(DataSink& sink);

		void Clear()
		{
			ScopedLock lock(m_lock); 
			KeyHash().swap(m_map);
		}

		bool GetValue(const ConfigurationKey& key,ConfigurationValue *outValue)
		{
			KeyHash::const_iterator it;

			ScopedLock lock(m_lock); 
			it = m_map.find(key);
			if(it != m_map.end()) {
				*outValue = it->second;
				return true;
			}
			return false;
		}

		void SetValue(const ConfigurationKey& key,const ConfigurationValue& value)
		{
			ScopedLock lock(m_lock);
			m_map[key] = value;
			m_changed = true;
		}
		
		void UnsetValue(const ConfigurationKey& key) 
		{
			ScopedLock lock(m_lock);
			KeyHash::iterator it;
			it = m_map.find(key);
			if(it != m_map.end()) m_map.erase(it);
			m_changed = true;
		}
	};
	
	typedef std::tr1::shared_ptr<Configuration> ConfigurationPtr;
}

#endif
