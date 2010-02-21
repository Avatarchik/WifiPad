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
#if __APPLE__
#include <TargetConditionals.h>
#include <dns_sd.h>
#if TARGET_OS_IPHONE
#include <CoreGraphics/CoreGraphics.h>
#else
#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>
#endif
#endif
#include <stdlib.h>
#include <time.h>
#include <memory>
#include <stdexcept>
#include <list>
#include <boost/unordered_map.hpp>
#include <time.h>
#include <sys/stat.h>
#if __APPLE__
#include "QuartzRef.h"
#endif
#include "Directory.h"
#include "OS.h"
#include "Device.h"
#include "DataSource.h"
#include "DataSink.h"
#include "FileDataSource.h"
#include "FileDataSink.h"
#include "Configuration.h"
#include "Protocol.h"
#include "KeySyms.h"
#include "Server.h"

#if linux
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#endif

namespace WifiPad
{
	
#ifdef _WIN32
	void SimulateMouse(int button,int state,int dx = 0,int dy = 0,int dz = 0) {
		INPUT input;
		input.type = INPUT_MOUSE;
		
		input.mi.dx = dx;
		input.mi.dy = dy;
		input.mi.dwFlags = 0;
		switch(button) {
			case 0: input.mi.dwFlags |= state ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_LEFTUP; break;
			case 1: input.mi.dwFlags |= state ? MOUSEEVENTF_RIGHTDOWN : MOUSEEVENTF_RIGHTUP; break;
			case 2: input.mi.dwFlags |= state ? MOUSEEVENTF_MIDDLEDOWN : MOUSEEVENTF_MIDDLEUP; break;
		}
		input.mi.dwFlags |= (dx | dy) ? MOUSEEVENTF_MOVE : 0;
		if(dz) input.mi.dwFlags |= MOUSEEVENTF_WHEEL;
		input.mi.dwExtraInfo = 0;
		input.mi.mouseData = dz;
		input.mi.time = 0;
		
		::SendInput(1,&input,sizeof(input));
	}

	/* the plaform specific function which simulates keys! */
	void SimulateKey(int key,int state) {
		INPUT input;
		input.type = INPUT_KEYBOARD;
		
		input.ki.wVk = key;
		input.ki.wScan = ::MapVirtualKey(input.ki.wVk,MAPVK_VK_TO_VSC);
		input.ki.dwFlags = KEYEVENTF_SCANCODE | (state ? 0 : KEYEVENTF_KEYUP);

		switch(key) {
			case VK_UP:
			case VK_DOWN:
			case VK_LEFT:
			case VK_RIGHT:
			case VK_LCONTROL:
			case VK_RCONTROL:
			case VK_HOME:
			case VK_END:
			case VK_INSERT:
			case VK_DELETE:
			case VK_PRIOR:
			case VK_NEXT:
			case VK_LWIN:
				input.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
				break;
		}
		input.ki.time = 0;
		input.ki.dwExtraInfo = 0;
		::SendInput(1,&input,sizeof(input));
	}
#endif
#if __APPLE__ 
#if !TARGET_OS_IPHONE && !TARGET_IPHONE_SIMULATOR
	void SimulateMouse(int button,int state,int dx = 0,int dy = 0,int dz = 0)
	{
		//QuartzRef<CGEventSourceRef> source = CGEventSourceCreate(kCGEventSourceStatePrivate); // Tiger fix
		CFRelease(CGEventCreate(NULL)); // Tiger fix

		CGEventType type;
		static int btnState = -1;
		if((dx | dy) == 0) {
			switch(button) {
				case 0: type = state ? kCGEventLeftMouseDown : kCGEventLeftMouseUp; break;
				case 1: type = state ? kCGEventRightMouseDown : kCGEventRightMouseUp; break;
				case 2: type = state ? kCGEventOtherMouseDown : kCGEventOtherMouseUp; break;
			}
			if(state) btnState = button;
			else btnState = -1;
		} else {
			switch(btnState) {
				case 0: type = kCGEventLeftMouseDragged; break;
				case 1: type = kCGEventRightMouseDragged; break;
				case 2: type = kCGEventOtherMouseDragged; break;
				default: type = kCGEventMouseMoved; break;
			}
		}
		
		// get mouse pos
		Point pt;
#if __LP64__
		HIPoint hipt;
		HIGetMousePosition(kHICoordSpaceScreenPixel,NULL,&hipt);
		pt.h = hipt.x;
		pt.v = hipt.y;
#else
		GetGlobalMouse(&pt);
#endif
		
		// convert to cgpoint
		CGPoint npt;
		npt.x = pt.h + dx;
		npt.y = pt.v + dy;
		
		const CGDirectDisplayID mainDisplay = CGMainDisplayID();
		const int xres = CGDisplayPixelsWide(mainDisplay);
		const int yres = CGDisplayPixelsHigh(mainDisplay);
		if(npt.x < 0) npt.x = 0;
		else if(npt.x > xres) npt.x = xres - 1;
		if(npt.y < 0) npt.y = 0;
		else if(npt.y > yres) npt.y = yres - 1;
		
		QuartzRef<CGEventRef> event = CGEventCreateMouseEvent(NULL,type,npt,button);
		if(event) CGEventPost(kCGSessionEventTap,event);
		
		if(dz) {
			QuartzRef<CGEventRef> event = CGEventCreate(NULL);
			if(event) {
				CGEventSetType(event,kCGEventScrollWheel);
				CGEventSetIntegerValueField(event,kCGScrollWheelEventDeltaAxis1,dz);
				CGEventPost(kCGSessionEventTap,event);
			}
		}
	}
	
	void SimulateKey(int key,int state) {
		//QuartzRef<CGEventSourceRef> source = CGEventSourceCreate(kCGEventSourceStatePrivate); // Tiger fix
		CFRelease(CGEventCreate(NULL)); // Tiger fix

		switch(key) {
			case KeySyms::LBUTTON: SimulateMouse(0,state); break;
			case KeySyms::RBUTTON: SimulateMouse(1,state); break;
			case KeySyms::MBUTTON: SimulateMouse(2,state); break;
			case KeySyms::MOUSEUP: SimulateMouse(-1,0,0,-state); break;
			case KeySyms::MOUSEDOWN: SimulateMouse(-1,0,0,state); break;
			case KeySyms::MOUSELEFT: SimulateMouse(-1,0,-state,0); break;
			case KeySyms::MOUSERIGHT: SimulateMouse(-1,0,state,0); break;
			case KeySyms::WHEELUP: SimulateMouse(-1,0,0,-state); break;
			case KeySyms::WHEELDOWN: SimulateMouse(-1,0,0,state); break;
			default:
				{
					QuartzRef<CGEventRef> event = CGEventCreateKeyboardEvent(NULL,key,state ? true : false);
					if(event) {
						CGEventSetType(event,state ? kCGEventKeyDown : kCGEventKeyUp);
						CGEventPost(kCGSessionEventTap,event);
					}
					break;
				}
		}
	}
#else
	inline void SimulateKey(int key,int state) {
	}
	
	inline void SimulateMouse(int button,int state,int dx = 0,int dy = 0,int dz = 0)
	{
	}
#endif
#endif
#if linux     
		class XDisplayHandle
        {
        private:
            Display *m_xdpy;
        public:
			XDisplayHandle() { m_xdpy = XOpenDisplay(0); }
            ~XDisplayHandle() { XCloseDisplay(m_xdpy); }

            operator Display *() { return m_xdpy; }
        };

		void SimulateMouse(int button,int state,int dx = 0,int dy = 0,int dz = 0)
		{
			XDisplayHandle display;

			if(button >= 0) {
				switch(button) {
					case 0: XTestFakeButtonEvent(display,1,state ? 1 : 0,0); break; // left
					case 1: XTestFakeButtonEvent(display,3,state ? 1 : 0,0); break; // right
					case 2: XTestFakeButtonEvent(display,2,state ? 1 : 0,0); break; // middle
				}


			}

			XTestFakeRelativeMotionEvent(display,dx,dy,0);
		}

        void SimulateKey(int key,int state) {
			XDisplayHandle display; // xdiplay handles not valid across thread, so create one in the thread
#if 0
			XKeyEvent event;
            event.display = display;
            event.root = RootWindow((Display *)display,DefaultScreen((Display *)display));
            event.subwindow = None;
            event.window = event.root;
            event.time = CurrentTime;
            event.x = 1;
            event.y = 1;
            event.x_root = 1;
            event.y_root = 1;
            event.state = 0;
            event.keycode = XKeysymToKeycode(display,key);
            event.same_screen = True;
            event.type = state > 0 ? KeyPress : KeyRelease;
            if(event.keycode != 0) XSendEvent(display,InputFocus,TRUE,KeyPressMask : KeyReleaseMask,(XEvent *)&event);
#endif
            int code = XKeysymToKeycode(display,key);
            if(code) XTestFakeKeyEvent(display,code,state > 0,0);
        }
#endif
	
	Server::Server(const std::string& hostname,int port) : 
		m_serverName(""), m_mouseSpeed(5), m_terminateFlag(false)
	{
		m_exePath = OS::GetResourcesPath();
		m_appDataPath = OS::GetAppDataPath();

		// create application data directories
		OS::CreateDirectory(GetAppDataPath(),0750);
		OS::CreateDirectory(GetConfigurationsPath(),0750);
		OS::CreateDirectory(GetGamePadsPath(),0750);

		LoadGamePads();
		
		try {
			m_clientSocket.Bind(hostname,port);
			m_clientSocket.Listen(4);
			m_dataSocket.Bind("0.0.0.0",port,Socket::UDP);
			
#if __APPLE__
			m_dnsServiceRef = 0;
			DNSServiceRegister(&m_dnsServiceRef,0,0,NULL,"_wifipad._tcp",NULL,NULL,htons(8989),0,NULL,NULL,NULL);
#endif
		} catch(const std::runtime_error&) {
			throw std::runtime_error("Failed to listen to on port 8989. Please make sure all clients are disconnected and try again in a few minutes.");
		}
	}

	Server::~Server()
	{
#if __APPLE__
		if(m_dnsServiceRef) DNSServiceRefDeallocate(m_dnsServiceRef);
#endif
		Stop();
	}
	
	struct MatchUuid : std::unary_function<GamePadMeta, bool>
	{
		const Uuid& left;
		MatchUuid(const Uuid& uuid) : left(uuid) { }
		
		bool operator ()(const GamePadMeta& right) const
		{
			return left == right.gamePad->GetUuid();
		}
	};
	
	bool Server::HandleQuery(Socket& socket)
	{
		Packet query, response;

		if(!query.Read(socket)) return false;
		switch(query.command) {
			case PacketCommand::ACQUIRE_SESSION:
			{
				// locate existing session and delete it if it exists
				{
					ScopedLock lock(m_devicesLock);
					for(boost::unordered_map<uint32_t,DevicePtr>::iterator 
						it = m_devices.begin(); it != m_devices.end(); ) 
					{
						if(it->second->GetDeviceUuid() == query.packet.acquireSession.deviceUuid)
							it = m_devices.erase(it);
						else 
							++it;
					}
				}

				// generate session
				// notice that sessionID is not a security feature, thus, no crypto algo is used
				static uint32_t sessionId = 0;
				static Mutex sessionIdLock;
				{
					ScopedLock lock(sessionIdLock);
					if(sessionId == 0) {
						srand((unsigned)time(NULL));
						sessionId = rand(); // randomize initial sessionID
					}
					sessionId++;
				}
				
				// null terminate to prevent string overflow
				query.packet.acquireSession.name[31] = 0;

				// grab the device
				DevicePtr device;
				try {
					// if device is already exists in server, give it new session id
					device = GetDeviceByUuid(query.packet.acquireSession.deviceUuid);
					device->SetSessionId(sessionId);
				} catch(const std::runtime_error& ) {
					// if no device exists, add it to device list
					device.reset(new Device(query.packet.acquireSession.name,query.packet.acquireSession.deviceUuid,sessionId));
				}

				{
					ScopedLock lock(m_devicesLock);
					m_devices.insert(std::pair<uint32_t,DevicePtr>(sessionId,device));
				}
				
				// return session id
				response.packet.sessionID.sessionID = htonl(sessionId);
				response.Send<SessionIDPacket>(socket);

				break;
			}
			case PacketCommand::LIST_GAMEPADS:
			{
				ScopedLock lock(m_gamePadListLock);
				
				// check session
				try {
					GetDeviceBySessionId(ntohl(query.packet.listGamePads.sessionID));
				} catch(const std::runtime_error&) {
					response.packet.error.errorCode = htonl(1);
					strncpy(response.packet.error.message,"Invalid session.",31);
					response.Send<ErrorPacket>(socket);
					break;
				}
				
				// get number of gamepads
				response.packet.gamePadList.numGamePads = htonl(m_gamePadList.size());
				response.Send<GamePadListPacket>(socket);

				GamePadInfoPacket info;
				for(GamePadList::const_iterator it = m_gamePadList.begin();
					it != m_gamePadList.end(); ++it) {
					info.gamePadUuid = it->gamePad->GetUuid();
					strncpy(info.name,it->gamePad->GetName().c_str(),31);
					info.name[31] = 0;
					socket.WriteFully(&info,sizeof(GamePadInfoPacket));
				}
			}
			break;
			case PacketCommand::SELECT_GAMEPAD:
			{		
				// check session
				DevicePtr device;
				try {
					device = GetDeviceBySessionId(ntohl(query.packet.selectGamePad.sessionID));
				} catch(const std::runtime_error&) {
					response.packet.error.errorCode = htonl(1);
					strncpy(response.packet.error.message,"Invalid session.",31);
					response.Send<ErrorPacket>(socket);
					break;
				}

				std::string pathToGamePad;
				// find path to gamepad
				try {
					pathToGamePad = GetGamePadPathByUuid(query.packet.selectGamePad.gamePadUuid);
					device->SetGamePad(LoadGamePadByUuid(query.packet.selectGamePad.gamePadUuid,0));
				} catch(const std::runtime_error&) {
					response.packet.error.errorCode = htonl(1);
					strncpy(response.packet.error.message,"Gamepad was not found.",31);
					response.Send<ErrorPacket>(socket);
					break;
				}
				
				// determine gamepad filesize
				struct stat buf;
				if(stat(pathToGamePad.c_str(),&buf) != 0) {
					response.packet.error.errorCode = htonl(1);
					strncpy(response.packet.error.message,"Gamepad removed from server.",31);
					response.Send<ErrorPacket>(socket);
					break;
				}
				
				try {
					// setup source 
					FileDataSource source(pathToGamePad);
					uint8_t buffer[1024];

					// gamepad response
					response.packet.gamePadData.dataSize = htonl(buf.st_size);
					response.Send<GamePadDataPacket>(socket);

					// gamepad data
					int readBytes;
					while((readBytes = source.ReadBytes(buffer,1024)) > 0) {
						socket.WriteFully(buffer,readBytes);
					}
				} catch(const std::runtime_error&) { 
					// shouldn't happen, but if gamepad is removed while its trying to send it, terminate socket
					socket.Close();
				}
			}
			break;
			case PacketCommand::LIST_CONFIGURATIONS:
			{
				// check session
				try {
					GetDeviceBySessionId(ntohl(query.packet.listConfigurations.sessionID));
				} catch(const std::runtime_error&) {
					response.packet.error.errorCode = htonl(1);
					strncpy(response.packet.error.message,"Invalid session.",31);
					response.Send<ErrorPacket>(socket);
					break;
				}
				
				ConfigurationList configList;
				GetConfigurationList(&configList);
				response.packet.configurationList.numConfigurations = htonl(configList.size() + 1);
				response.Send<ConfigurationListPacket>(socket);

				ConfigurationInfoPacket info;
				strncpy(info.name,"<Default>",31);
				socket.WriteFully(&info,sizeof(ConfigurationInfoPacket));
				for(ConfigurationList::const_iterator it = configList.begin();
					it != configList.end(); ++it) {
					strncpy(info.name,it->c_str(),31);
					info.name[31] = 0;
					socket.WriteFully(&info,sizeof(ConfigurationInfoPacket));
				}
			}
			break;
			case PacketCommand::SELECT_CONFIGURATION:
			{		
				// check session
				DevicePtr device;
				try {
					device = GetDeviceBySessionId(ntohl(query.packet.selectConfiguration.sessionID));
				} catch(const std::runtime_error&) {
					response.packet.error.errorCode = htonl(1);
					strncpy(response.packet.error.message,"Invalid session.",31);
					response.Send<ErrorPacket>(socket);
					break;
				}

				try {
					query.packet.selectConfiguration.name[31] = 0;
					if(strcmp("<Default>",query.packet.selectConfiguration.name) == 0)
						device->SetConfiguration(ConfigurationPtr());
					else
						device->SetConfiguration(LoadConfiguration(query.packet.selectConfiguration.name));
					response.Send<ConfigurationLoadedPacket>(socket);
				} catch(const std::runtime_error&) {
					response.packet.error.errorCode = htonl(1);
					strncpy(response.packet.error.message,"Configuration not found.",31);
					response.Send<ErrorPacket>(socket);
					break;
				}
			}
			break;
		}

		return true;
	}

	int Server::ServerThread(void *data)
	{
		Packet query, response;
		IP ip;
		std::list<int> repeatKeys;
		float mouseAccel = 0;

		while(!m_terminateFlag) {
			if(!repeatKeys.empty()) {
				mouseAccel *= 1.05f;
				if((int)mouseAccel >= m_mouseSpeed) mouseAccel = m_mouseSpeed; 
				timeval tv;
				tv.tv_sec = 0;
				tv.tv_usec = 1000;
				if(!m_dataSocket.Poll(tv)) {
					for(std::list<int>::const_iterator it = repeatKeys.begin();
						it != repeatKeys.end();
						++it) {
						switch(*it) {
							case KeySyms::LBUTTON: SimulateMouse(0,(int)mouseAccel); break;
							case KeySyms::RBUTTON: SimulateMouse(1,(int)mouseAccel); break;
							case KeySyms::MBUTTON: SimulateMouse(2,(int)mouseAccel); break;
							case KeySyms::MOUSEUP: SimulateMouse(-1,0,0,-(int)mouseAccel); break;
							case KeySyms::MOUSEDOWN: SimulateMouse(-1,0,0,(int)mouseAccel); break;
							case KeySyms::MOUSELEFT: SimulateMouse(-1,0,-(int)mouseAccel,0); break;
							case KeySyms::MOUSERIGHT: SimulateMouse(-1,0,(int)mouseAccel,0); break;
							case KeySyms::WHEELUP: SimulateMouse(-1,0,0,0,(int)mouseAccel); break;
							case KeySyms::WHEELDOWN: SimulateMouse(-1,0,0,0,-(int)mouseAccel); break;
							default: SimulateKey(*it,(int)mouseAccel); break;
						}
					}
					continue;
				}
			}
			int size = m_dataSocket.RecvFrom((uint8_t *)&query,sizeof(Packet),&ip);
			if(size >= 16) {
				switch(ntohl(query.command)) {
					case PacketCommand::DISCOVER:
					{
						if(ntohl(query.packet.discover.magic) == 0x1189AF80) {
							response.command = htonl(PacketCommand::SERVER_INFO);
							response.size = htonl(sizeof(ServerInfoPacket));
							response.timestamp = htonll(time(NULL));
							response.packet.serverInfo.version = htonl(1);
							strncpy(response.packet.serverInfo.name,m_serverName.c_str(),32);
							response.packet.serverInfo.name[31] = 0;
							m_dataSocket.SendTo(&response,sizeof(ServerInfoPacket) + 16,ip,8990);
						}
					}

					break;
					case PacketCommand::BUTTON_EVENT:
					{
						try {
							// get device given session
							DevicePtr device = GetDeviceBySessionId(ntohl(query.packet.buttonEvent.sessionID));
							
							GamePadPtr gamePad = device->GetGamePad();
							if(!gamePad) break;
							
							ConfigurationKey key;
							ConfigurationValue value;
							key.gamepadUUID = gamePad->GetUuid();
							key.buttonNo = ntohl(query.packet.buttonEvent.buttonNo);
							
							ConfigurationPtr configuration = device->GetConfiguration();

							int keysToSimulate[3];
							if(configuration && configuration->GetValue(key,&value)) {
								keysToSimulate[0] = value.key[0];
								keysToSimulate[1] = value.key[1];
								keysToSimulate[2] = value.key[2];
							} else {
								const ButtonInfo& button = gamePad->GetButton(key.buttonNo);
								keysToSimulate[0] = button.defaultKey[0];
								keysToSimulate[1] = button.defaultKey[1];
								keysToSimulate[2] = button.defaultKey[2];
							}

							for(int i = 0; i < 3; i++) {
								int state = ntohl(query.packet.buttonEvent.buttonState);
								
								// repeat mouse keys
								if(keysToSimulate[i] >= KeySyms::MOUSEUP && keysToSimulate[i] <= KeySyms::WHEELDOWN) {
									mouseAccel = 1.0;
									if(state) {
										repeatKeys.push_back(keysToSimulate[i]);
										state = (int)mouseAccel;
									}
									else repeatKeys.remove(keysToSimulate[i]);
								}

								switch(keysToSimulate[i]) {
									case KeySyms::LBUTTON: SimulateMouse(0,state); break;
									case KeySyms::RBUTTON: SimulateMouse(1,state); break;
									case KeySyms::MBUTTON: SimulateMouse(2,state); break;
									case KeySyms::MOUSEUP: SimulateMouse(-1,0,0,-state); break;
									case KeySyms::MOUSEDOWN: SimulateMouse(-1,0,0,state); break;
									case KeySyms::MOUSELEFT: SimulateMouse(-1,0,-state,0); break;
									case KeySyms::MOUSERIGHT: SimulateMouse(-1,0,state,0); break;
									case KeySyms::WHEELUP: SimulateMouse(-1,0,0,0,state); break;
									case KeySyms::WHEELDOWN: SimulateMouse(-1,0,0,0,-state); break;
									case -1: break;
									default: SimulateKey(keysToSimulate[i],state); break;
								}
							}
						} catch(const std::runtime_error&) {
						}
					}
					break;
					case PacketCommand::TRACKPAD_EVENT:
					{
						try {
							// get device given session
							DevicePtr device = GetDeviceBySessionId(ntohl(query.packet.trackPadEvent.sessionID));
							
							int dx = ntohl(query.packet.trackPadEvent.dx);
							int dy = ntohl(query.packet.trackPadEvent.dy);
							int dz = ntohl(query.packet.trackPadEvent.dz);

							if(dx < -1 || dx > 1) dx = dx * (1.0f - (1.0f / abs(dx))) * m_mouseSpeed;
							if(dy < -1 || dy > 1) dy = dy * (1.0f - (1.0f / abs(dy))) * m_mouseSpeed;
							if(dz < -1 || dz > 1) dz = dz * (1.0f - (1.0f / abs(dz))) * m_mouseSpeed;
							
							SimulateMouse(-1,0,dx,dy,dz);
						} catch(const std::runtime_error&) {
						}
					}
					break;
					case PacketCommand::ACCELEROMETER_EVENT:
					{
						try {
							// get device given session
							DevicePtr device = GetDeviceBySessionId(ntohl(query.packet.accelerometerEvent.sessionID));
							
							int ax = ntohl(query.packet.accelerometerEvent.x);
							int ay = ntohl(query.packet.accelerometerEvent.y);
							int az = ntohl(query.packet.accelerometerEvent.z);
							
							// FIXME: okay, figure out what should we simulate for accelerometer events
							
						} catch(const std::runtime_error&) {
						}
					}
					break;
				}
			} else {
				// DS2Key (http://www.1emulation.com/forums/index.php?showtopic=28860) is awesome,
				// it certainly beats using an iPhone as a gamepad
				// but they don't have a mac version...
				// So I added it here into WifiPad for my personal usage.
				// To configure it, add a configuration called "DS2Key"
				// Keyboard support only.
				char *msg = (char *)&query;
				if(msg[0] == '/') {
					static ConfigurationPtr configuration;
					static GamePadPtr gamePad;
					static bool loadOnce = false;
					
					if(!loadOnce) {
						try {
							configuration = LoadConfiguration("DS2Key");
							Uuid uuid;
							uuid.ParseString("16F628FA-215C-49D8-B651-E3DB524E3A3E");
							gamePad = LoadGamePadByUuid(uuid,0);
						} catch(const std::runtime_error& ) {
							continue;
						}
						loadOnce = true;
					}
					
					int button, state;
					if(!strncmp(msg,"/du0",4)) { button = 0; state = 0xFFFF; }
					else if(!strncmp(msg,"/du1",4)) { button = 0; state = 0; }
					else if(!strncmp(msg,"/dr0",4)) { button = 2; state = 0xFFFF; }
					else if(!strncmp(msg,"/dr1",4)) { button = 2; state = 0; }
					else if(!strncmp(msg,"/dd0",4)) { button = 4; state = 0xFFFF; }
					else if(!strncmp(msg,"/dd1",4)) { button = 4; state = 0; }
					else if(!strncmp(msg,"/dl0",4)) { button = 6; state = 0xFFFF; }
					else if(!strncmp(msg,"/dl1",4)) { button = 6; state = 0; }
					else if(!strncmp(msg,"/ba0",4)) { button = 8; state = 0xFFFF; }
					else if(!strncmp(msg,"/ba1",4)) { button = 8; state = 0; }
					else if(!strncmp(msg,"/bb0",4)) { button = 9; state = 0xFFFF; }
					else if(!strncmp(msg,"/bb1",4)) { button = 9; state = 0; }
					else if(!strncmp(msg,"/bx0",4)) { button = 10; state = 0xFFFF; }
					else if(!strncmp(msg,"/bx1",4)) { button = 10; state = 0; }
					else if(!strncmp(msg,"/by0",4)) { button = 11; state = 0xFFFF; }
					else if(!strncmp(msg,"/by1",4)) { button = 11; state = 0; }
					else if(!strncmp(msg,"/bl0",4)) { button = 12; state = 0xFFFF; }
					else if(!strncmp(msg,"/bl1",4)) { button = 12; state = 0; }
					else if(!strncmp(msg,"/br0",4)) { button = 13; state = 0xFFFF; }
					else if(!strncmp(msg,"/br1",4)) { button = 13; state = 0; }
					else if(!strncmp(msg,"/bt0",4)) { button = 14; state = 0xFFFF; }
					else if(!strncmp(msg,"/bt1",4)) { button = 14; state = 0; }
					else if(!strncmp(msg,"/be0",4)) { button = 15; state = 0xFFFF; }
					else if(!strncmp(msg,"/be1",4)) { button = 15; state = 0; }
					else { msg[0] = 0; continue; }
					
					int keysToSimulate[3];
					ConfigurationKey key;
					ConfigurationValue value;
					key.gamepadUUID = gamePad->GetUuid();
					key.buttonNo = button;
					
					if(configuration && configuration->GetValue(key,&value)) {
						keysToSimulate[0] = value.key[0];
						keysToSimulate[1] = value.key[1];
						keysToSimulate[2] = value.key[2];
					} else {
						const ButtonInfo& buttonInfo = gamePad->GetButton(button);
						keysToSimulate[0] = buttonInfo.defaultKey[0];
						keysToSimulate[1] = buttonInfo.defaultKey[1];
						keysToSimulate[2] = buttonInfo.defaultKey[2];
					}
					
					for(int i = 0; i < 3; i++) {
						SimulateKey(keysToSimulate[i],state);
					}
				}
				msg[0] = 0;
			}
		}
		return 0;
	}

	int Server::ClientThread(void *data)
	{
		Socket *clientSocket = (Socket *)data;
		while(!m_terminateFlag && HandleQuery(*clientSocket)) { }
		return 0;
	}

	int Server::ListenerThread(void *data)
	{
		try {
			Socket socket; // accept next socket
			m_clientSocket.Accept(&socket);
			
			std::list<Socket *>::iterator curSocket; // insert it to client list
			{ 
				ScopedLock lock(m_clientListLock);
				curSocket = m_clientList.insert(m_clientList.end(),&socket);
			}

			// start client thread
			Thread<Server> clientThread(this,&Server::ClientThread,&socket);
			
			// promote next thread to accept another connection
			Thread<Server> nextListenerThread(this,&Server::ListenerThread,data);
			
			// wait for client to be finished
			clientThread.Join();
			{
				ScopedLock lock(m_clientListLock);
				m_clientList.erase(curSocket); // remove socket for connection list
			}
			
			nextListenerThread.Join();
		} catch(const std::runtime_error& ) {
		}

		return 0;
	}

	void Server::Start()
	{
		if(!m_serverThread.get())
			m_serverThread.reset(new Thread<Server>(this,&Server::ServerThread,NULL));
		if(!m_listenerThread.get())
			m_listenerThread.reset(new Thread<Server>(this,&Server::ListenerThread,NULL));
	}

	void Server::Stop()
	{
		m_terminateFlag = true;
		
		m_dataSocket.Close();
		m_serverThread->Join();
		
		// close all client sockets
		m_clientSocket.Close();
		{
			ScopedLock lock(m_clientListLock);
			for(std::list<Socket *>::iterator it = m_clientList.begin();
				it != m_clientList.end(); ++it) {
				(*it)->Close();
			}
		}
		m_listenerThread->Join();
	}

	void Server::GetConfigurationList(ConfigurationList *outConfigurationList) const
	{
		ScopedLock lock(m_configurationLock);
		outConfigurationList->clear();
		std::string path =  GetAppDataPath() + "/Configurations";
#if _WIN32
		Directory dir(path + "/*.config");
#else
		Directory dir(path + "/");
#endif
		while(dir)
		{
			const int filenameLength = dir.path.length();
			if(filenameLength > 7 && dir.path.substr(filenameLength - 7) == ".config") 
				outConfigurationList->push_back(dir.path.substr(0, filenameLength - 7));
			++dir;
		}
	}
	
	ConfigurationPtr Server::LoadConfiguration(const std::string& configurationName)
	{
		ScopedLock lock(m_configurationLock);
		FileDataSource configFile(GetConfigurationsPath() + "/" + configurationName + ".config");
		ConfigurationPtr configurationLoad(new Configuration());
		configurationLoad->Load(configurationName, configFile);
		return configurationLoad;
	}
	
	void Server::SaveConfiguration(const ConfigurationPtr& configuration,const std::string& configurationName)
	{
		ScopedLock lock(m_configurationLock);
		FileDataSink configFile(GetConfigurationsPath() + "/" + configurationName + ".config");
		configuration->Save(configFile);
	}
	
	void Server::DeleteConfiguration(const std::string& configutationName)
	{
		ScopedLock lock(m_configurationLock);
		OS::DeleteFile(GetConfigurationsPath() + "/" + configutationName + ".config");
	}
	

	void Server::LoadGamePads()
	{
		ScopedLock lock(m_gamePadListLock);

		// attempt to grab default gamepad
		std::vector<GamePadMeta>().swap(m_gamePadList);
		
		GamePadMeta info;
		info.path = OS::GetResourcesPath() + "/Default.gamepad";
		FileDataSource source(info.path);
		info.gamePad->LoadGamePad(source,0);
		m_gamePadList.push_back(info);
		
		std::string path = GetGamePadsPath();
#if _WIN32
		Directory dir(path + "/*.gamepad");
#else
		Directory dir(path + "/");
#endif
		while(dir) {
			try {
				info.path = path + "/" + dir.path;
				FileDataSource source(info.path);
				info.gamePad.reset(new GamePad(source,0));

				// check if there is a duplicate gamepad with the same uuid
				try {
					std::string dup = GetGamePadPathByUuid(info.gamePad->GetUuid());

					// if no exception is raised, the there is a duplciate, don't bother loading it
					++dir;
					continue;
				} catch(const std::runtime_error&) {

				}
				m_gamePadList.push_back(info);
			} catch(const std::runtime_error&) { // ignore errors
			}
			++dir;
		}
	}
	
	void Server::GetGamePadList(GamePadList *outGamepadList) const 
	{ 
		ScopedLock lock(m_gamePadListLock);
		outGamepadList->clear();
		*outGamepadList = m_gamePadList; 
	}
	
	GamePadPtr Server::LoadGamePadByUuid(const Uuid& uuid,int loadLevel)
	{
		FileDataSource source(GetGamePadPathByUuid(uuid));
		return GamePadPtr(new GamePad(source,loadLevel));
	}
	
	std::string Server::GetGamePadPathByUuid(const Uuid& uuid)
	{
		std::vector<GamePadMeta>::const_iterator gamePad =
			std::find_if(m_gamePadList.begin(),m_gamePadList.end(),MatchUuid(uuid));
		if(gamePad == m_gamePadList.end()) throw std::runtime_error("No gamepad by that uuid.\n");
		return gamePad->path;
	}
	
	void Server::GetDeviceList(DeviceList *outDeviceList) const 
	{ 
		ScopedLock lock(m_devicesLock);
		outDeviceList->clear();
		for(boost::unordered_map<uint32_t,DevicePtr>::const_iterator it = 
			m_devices.begin(); it != m_devices.end(); ++it) {
			outDeviceList->push_back(it->second);
		}
	}
	
	DevicePtr Server::GetDeviceBySessionId(uint32_t sessionId)
	{
		ScopedLock lock(m_devicesLock);
		boost::unordered_map<uint32_t,DevicePtr>::iterator device =
			m_devices.find(sessionId);
		if(device == m_devices.end()) {
			throw std::runtime_error("No device found by session ID.\n");
		}
		return device->second;
	}
	
	struct MatchDeviceUuid : std::unary_function<DevicePtr, bool>
	{
		const Uuid& left;
		MatchDeviceUuid(const Uuid& uuid) : left(uuid) { }
		
		bool operator ()(const std::pair<uint32_t,DevicePtr>& right) const
		{
			return left == right.second->GetDeviceUuid();
		}
	};
	
	DevicePtr Server::GetDeviceByUuid(const Uuid& uuid)
	{
		ScopedLock lock(m_devicesLock);
		boost::unordered_map<uint32_t,DevicePtr>::const_iterator device =
			std::find_if(m_devices.begin(),m_devices.end(),MatchDeviceUuid(uuid));
		if(device == m_devices.end()) throw std::runtime_error("Device not connected.\n");
		return device->second;
	}
}
