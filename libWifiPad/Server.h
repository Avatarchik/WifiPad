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
#ifndef __Server_h
#define __Server_h

#include <string>
#include <vector>
#include <list>
#include <memory>
#include <boost/unordered_map.hpp>
#include "Uuid.h"
#include "Socket.h"
#include "Protocol.h"
#include "Configuration.h"
#include "FileDataSource.h"
#include "GamePad.h"
#include "Device.h"
#include "Mutex.h"
#include "Thread.h"

namespace WifiPad
{
	struct GamePadMeta
	{
		GamePadPtr gamePad;
		std::string path;
		
		GamePadMeta() : gamePad(new GamePad()) { }
	};
	
	class Server
	{
	public:
		typedef std::vector<DevicePtr> DeviceList;
		typedef std::vector<GamePadMeta> GamePadList;
		typedef std::vector<std::string> ConfigurationList;
	private:
		std::string m_serverName;

		int m_mouseSpeed;

		Socket m_clientSocket;
		Socket m_dataSocket;
		Configuration m_configuration;
		
		std::auto_ptr< Thread<Server> > m_serverThread;
		std::auto_ptr< Thread<Server> > m_listenerThread;

		std::vector<GamePadMeta> m_gamePadList; // loaded game pads
		boost::unordered_map<uint32_t,DevicePtr> m_devices; // devices
		
		std::list<Socket *> m_clientList;

		std::string m_exePath;
		std::string m_appDataPath;

		bool m_terminateFlag;

		mutable Mutex m_serverLock, m_gamePadListLock, m_devicesLock, m_configurationLock, m_clientListLock;

		bool HandleQuery(Socket& socket);
	public:
		Server(const std::string& hostname,int port);
		~Server();

		int ServerThread(void *data);
		int ClientThread(void *data);
		int ListenerThread(void *data);

		// starts the server
		void Start();

		// stops the server
		void Stop();

		std::string GetServerName() const { return m_serverName; }
		void SetServerName(const std::string& name) { m_serverName = name; }

		void SetMouseSpeed(int speed) { m_mouseSpeed = speed; }
		int GetMouseSpeed() const { return m_mouseSpeed; }

		// get list of available configurations
		void GetConfigurationList(ConfigurationList *outConfigurationList) const;
		
		ConfigurationPtr LoadConfiguration(const std::string& configurationName);
		void SaveConfiguration(const ConfigurationPtr& configuration,const std::string& configurationName);
		void DeleteConfiguration(const std::string& configutationName);
		
		// Loads up gamepads
		void LoadGamePads();
		// gets the list of gamepads
		void GetGamePadList(GamePadList *outGamepadList) const;
		GamePadPtr LoadGamePadByUuid(const Uuid& uuid,int loadLevel = 1);
		std::string GetGamePadPathByUuid(const Uuid& uuid);
		
		// obtains the device list
		void GetDeviceList(DeviceList *outDeviceList) const;
		// grabs device by uuid
		DevicePtr GetDeviceBySessionId(uint32_t sessionId);
		DevicePtr GetDeviceByUuid(const Uuid& uuid);

		// gets the path to the executable
		const std::string& GetExePath() const { return m_exePath; }

		// gets the path to the application data directory
		const std::string& GetAppDataPath() const { return m_appDataPath; }
		
		const std::string GetGamePadsPath() const { return m_appDataPath + "/Gamepads"; }
		const std::string GetConfigurationsPath() const { return m_appDataPath + "/Configurations"; }
	};
}

#endif
