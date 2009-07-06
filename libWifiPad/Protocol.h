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
#ifndef __Protocol_h
#define __Protocol_h

#include <vector>
#include <inttypes.h>
#include <time.h>
#include "Uuid.h"
#include "Socket.h"

namespace WifiPad
{
#if _WIN32
	using namespace boost;
#endif
	namespace PacketCommand
	{
		enum Command
		{
			DISCOVER,
			ACQUIRE_SESSION,
			LIST_GAMEPADS,
			SELECT_GAMEPAD,
			LIST_CONFIGURATIONS,
			SELECT_CONFIGURATION,
			BUTTON_EVENT,
			
			SERVER_INFO,
			SESSION_ID,
			GAMEPAD_LIST,
			CONFIGURATION_LIST,
			CONFIGURATION_LOADED,
			GAMEPAD,

			ERROR_PACKET,
			
			// v1.1
			TRACKPAD_EVENT
		};
	}
	
	struct DiscoverPacket
	{
		enum { Command = PacketCommand::DISCOVER };
		uint32_t magic; // 0x80AF8911
	};

	struct ButtonEventPacket
	{
		enum { Command = PacketCommand::BUTTON_EVENT };
		uint32_t sessionID;
		int32_t buttonNo;
		int32_t buttonState;
	};
	
	struct TrackPadEventPacket
	{
		enum { Command = PacketCommand::TRACKPAD_EVENT };
		uint32_t sessionID;
		int32_t dx; // delta x
		int32_t dy; // delta y
		int32_t dz; // delta z (wheel mouse)
	};

	struct AcquireSessionPacket 
	{
		enum { Command = PacketCommand::ACQUIRE_SESSION };
		Uuid deviceUuid; // device UUID
		char name[32]; // name of device
	};

	struct ListGamePadsPacket
	{
		enum { Command = PacketCommand::LIST_GAMEPADS };
		uint32_t sessionID;
	};

	struct SelectGamePadPacket
	{
		enum { Command = PacketCommand::SELECT_GAMEPAD };
		uint32_t sessionID;
		Uuid gamePadUuid;
	};

	struct ListConfigurationsPacket
	{
		enum { Command = PacketCommand::LIST_CONFIGURATIONS };
		uint32_t sessionID;
	};

	struct SelectConfigurationPacket
	{
		enum { Command = PacketCommand::SELECT_CONFIGURATION };
		uint32_t sessionID;
		char name[32];
	};

	struct ServerInfoPacket
	{
		enum { Command = PacketCommand::SERVER_INFO };
		uint32_t version;
		char name[32];
	};

	struct SessionIDPacket 
	{
		enum { Command = PacketCommand::SESSION_ID };
		uint32_t sessionID;
	};
	
	struct GamePadInfoPacket
	{
		Uuid gamePadUuid;
		char name[32];
	};
	
	struct GamePadListPacket
	{
		enum { Command = PacketCommand::GAMEPAD_LIST };
		uint32_t numGamePads;
	};
	
	struct ConfigurationInfoPacket
	{
		char name[32];
	};

	struct ConfigurationListPacket
	{
		enum { Command = PacketCommand::CONFIGURATION_LIST };
		uint32_t numConfigurations;
	};

	struct ConfigurationLoadedPacket
	{
		enum { Command = PacketCommand::CONFIGURATION_LOADED };
	};

	struct GamePadDataPacket
	{
		enum { Command = PacketCommand::GAMEPAD };
		uint32_t dataSize;
	};

	struct ErrorPacket
	{
		enum { Command = PacketCommand::ERROR_PACKET };
		int32_t errorCode;
		char message[32];
	};

	struct Packet
	{
		uint32_t command;
		uint32_t size;
		uint64_t timestamp;
		union {
			DiscoverPacket discover;
			ButtonEventPacket buttonEvent;
			AcquireSessionPacket acquireSession;
			ListGamePadsPacket listGamePads;
			SelectGamePadPacket selectGamePad;
			ListConfigurationsPacket listConfigurations;
			SelectConfigurationPacket selectConfiguration;
			ServerInfoPacket serverInfo;
			SessionIDPacket sessionID;
			GamePadInfoPacket gamePadInfo;
			GamePadListPacket gamePadList;
			ConfigurationInfoPacket configurationInfo;
			ConfigurationListPacket configurationList;
			ConfigurationLoadedPacket configurationLoaded;
			GamePadDataPacket gamePadData;
			ErrorPacket error;
			
			// v1.1
			TrackPadEventPacket trackPadEvent;
		} packet;

		bool Read(Socket& socket) {
			if(socket.ReadFully((uint8_t *)this,16) == -1) return false;
			this->command = ntohl(this->command);
			this->size = ntohl(this->size);
			this->timestamp = ntohll(this->timestamp);
			if(this->size <= sizeof(Packet) - 16) {
				if(socket.ReadFully((uint8_t *)&this->packet.discover,this->size) == -1)
					return false;
			}
			return true;
		}

		template<typename T>
		bool Send(Socket& socket) {
			this->command = htonl(T::Command);
			this->timestamp = htonll(time(NULL));
			this->size = htonl(sizeof(T));
			if(socket.WriteFully((uint8_t *)this,sizeof(T) + 16) == -1) return false;
			return true;
		}
	};
}

#endif
