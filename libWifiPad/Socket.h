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
#ifndef __Socket_h
#define __Socket_h

#if _WIN32
#include <windows.h>
#include <winsock.h>
#else
#include <arpa/inet.h>
#include <sys/time.h>
#endif
#include <string>
#include <inttypes.h>

#if _BIG_ENDIAN
#ifndef htonll
#define htonll(x) (x)
#endif
#ifndef ntohll
#define ntohll(x) (x)
#endif
#else
#ifndef htonll
#define htonll(x) ( ((uint64_t)htonl((uint32_t)(x)) << 32LL) | (uint64_t)htonl((uint32_t)((uint64_t)(x) >> 32LL)) )
#endif
#ifndef ntohll
#define ntohll(x) ( ((uint64_t)ntohl((uint32_t)(x)) << 32LL) | (uint64_t)ntohl((uint32_t)((uint64_t)(x) >> 32LL)) )
#endif
#endif

namespace WifiPad
{
	class IP
	{
	private:
		uint8_t m_ip[4];
	public:
		IP(){}
		IP(int a,int b,int c,int d) {
			m_ip[0] = a; m_ip[1] = b; m_ip[2] = c; m_ip[3] = d;
		}

		operator uint32_t() { return *(uint32_t *)&m_ip; }
		IP& operator =(uint32_t addr) { *(uint32_t *)&m_ip = addr; return *this; }
		const uint8_t& operator[](int n) const { return m_ip[n]; }
		
		bool operator ==(const IP& rhs) const { return *(uint32_t *)m_ip == *(uint32_t *)rhs.m_ip; }
	};

	class Socket
	{
	private:
		int m_socket;
	public:
		enum Protocol { TCP, UDP };

		// Creates a new socket baed on protocol
		explicit Socket(Socket::Protocol protocol = Socket::TCP);

		// Create socket from existing socket
		explicit Socket(int socket);

		// close socket
		~Socket();
		
		void SetNonBlocking(bool nonblocking);

		void Connect(const std::string& hostname,int port,timeval *tv = NULL);
		void Close();
		void Bind(const std::string& hostname,int port);
		void Listen(int backlog);
		Socket Accept();

		// Return -1, on end-of-file.
		int Read(void *buffer,int size);
		int ReadFully(void *buffer,int size);

		int Write(const void *buffer,int size) const;
		int WriteFully(const void *buffer,int size) const;

		int SendTo(const void *buffer,int size,IP ip,int port = -1);
		int RecvFrom(void *buffer,int size,IP *outIP);
		
		bool Poll(timeval& tv);
	};
}

#endif
