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
#ifndef __Device_h
#define __Device_h

#include <string>
#include <inttypes.h>
#include <boost/shared_ptr.hpp>
#include "Uuid.h"
#include "GamePad.h"
#include "Mutex.h"
#include "Configuration.h"

namespace WifiPad {
	
	// Represent a iPhone/iPod device
	class Device {
	private:
		std::string m_deviceName;
		Uuid m_deviceUuid;
		uint32_t m_sessionId;
		GamePadPtr m_currentGamePad;
		ConfigurationPtr m_currentConfiguration;
		mutable Mutex m_lock;
	public:
		Device(const std::string& deviceName,const Uuid deviceUuid,uint32_t sessionId): 
		  m_deviceName(deviceName), m_deviceUuid(deviceUuid), m_sessionId(sessionId)
		{
		}

		std::string GetDeviceName() const
		{
			ScopedLock lock(m_lock);
			return m_deviceName;
		}

		Uuid GetDeviceUuid() const
		{
			ScopedLock lock(m_lock);
			return m_deviceUuid;
		}

		uint32_t GetSessionId() const
		{
			ScopedLock lock(m_lock);
			return m_sessionId;
		}
		
		void SetSessionId(uint32_t sessionId)
		{
			ScopedLock lock(m_lock);
			m_sessionId = sessionId;
		}
		
		void SetGamePad(GamePadPtr gamePad)
		{
			ScopedLock lock(m_lock);
			m_currentGamePad = gamePad;
		}

		GamePadPtr GetGamePad() const
		{
			ScopedLock lock(m_lock);
			return m_currentGamePad;
		}
		
		void SetConfiguration(ConfigurationPtr configuration)
		{
			ScopedLock lock(m_lock);
			m_currentConfiguration = configuration;
		}

		ConfigurationPtr GetConfiguration() const
		{
			ScopedLock lock(m_lock);
			return m_currentConfiguration;
		}
	};
	
	typedef boost::shared_ptr<Device> DevicePtr;
}

#endif
