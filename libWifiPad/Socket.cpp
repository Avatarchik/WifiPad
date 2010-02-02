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
#include <stdexcept>
#include <signal.h>
#include <string.h>
#if _WIN32
#include <windows.h>
#include <winsock.h>
#else
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netdb.h>
#endif

class SocketInit
{
public:
	SocketInit() {
#if _WIN32
		WSADATA wsaData;
		if(WSAStartup(MAKEWORD(1,0),&wsaData)) {
			::MessageBox(NULL,"Error initializing winsock.","Error",MB_ICONERROR | MB_OK);	
			exit(1);
		}
#else
		signal(SIGPIPE,SIG_IGN);
#endif
	}

	~SocketInit() {
#if _WIN32
		WSACleanup();
#endif
	}
};

static SocketInit socketinit;


using namespace WifiPad;

Socket::Socket() : m_socket(-1)
{	
}

Socket::Socket(int sock) : m_socket(sock)
{
}


Socket::~Socket()
{
	Close();
}

void Socket::CreateSocket(Socket::Protocol protocol)
{
	this->m_socket = socket(AF_INET, protocol ==  Socket::TCP ? SOCK_STREAM : SOCK_DGRAM, 0);
	if(this->m_socket<0)
	{
		throw std::runtime_error("Unable to create socket.");
	}
#if _WIN32
	char option = '1';
#else
	int option = 1;
#if !linux
	setsockopt(m_socket,SOL_SOCKET,SO_NOSIGPIPE,&option,sizeof(option));
	
	timeval tv;
	tv.tv_sec = 10;
	tv.tv_usec = 0;
	setsockopt(m_socket,SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof(tv));
	setsockopt(m_socket,SOL_SOCKET,SO_SNDTIMEO,&tv,sizeof(tv));
#endif
#endif
	setsockopt(m_socket,SOL_SOCKET,SO_BROADCAST,&option,sizeof(option));
}

void Socket::SetNonBlocking(bool nonblocking)
{
	unsigned long nonblockingMode = nonblocking ? 1 : 0;
#if _WIN32
	ioctlsocket(m_socket,FIONBIO,&nonblockingMode);
#else 
	ioctl(m_socket,FIONBIO,&nonblockingMode);
#endif
}

void Socket::Connect(const std::string& hostname,int port,Socket::Protocol protocol,timeval *tv)
{
	Close();
	CreateSocket(protocol);
	
	hostent *host;
	sockaddr_in myAddress;
	host = gethostbyname(hostname.c_str());
	if(!host)
	{
		throw std::runtime_error("Failed to resolve hostname.");
	}
	memset((void *)&myAddress, 0, sizeof(myAddress));
	myAddress.sin_family = AF_INET;
	myAddress.sin_addr =  *((in_addr *)host->h_addr);
	myAddress.sin_port = htons(port);
	
	if(tv) {
		int res;
		int err = connect(m_socket,(sockaddr *)&myAddress, sizeof(myAddress));
		if(err < 0)
		{
#if _WIN32
			switch(WSAGetLastError()) {
				case WSAEINPROGRESS:
#else
			switch(errno) {
				case EINPROGRESS: 
#endif
					fd_set set;
					FD_ZERO(&set); 
					FD_SET(m_socket,&set); 
					res = select(m_socket + 1,NULL,&set,NULL,tv);
					if(res < 0) {
						throw std::runtime_error((std::string)"Could not connect to host: " + strerror(errno) + "\n");
					}
					
					if(res == 0) {
						throw std::runtime_error("Timeout on connecting to host.\n");
					}
					
					int error;
#if _WIN32
					int len;
#else
					socklen_t len;
#endif
					len = sizeof(int);
					if(getsockopt(m_socket,SOL_SOCKET,SO_ERROR,(char *)&error,&len) < 0) { 
						throw std::runtime_error((std::string)"Could not connect to host: " + strerror(errno) + "\n");
					} 
					
					if(error) {
						throw std::runtime_error((std::string)"Could not connect to host: " + strerror(error) + "\n");
					}
					
					break;
				default:
					throw std::runtime_error((std::string)"Could not connect to host: " + strerror(errno) + "\n");
			}
		}
	} else {
		if(connect(m_socket,(sockaddr *)&myAddress, sizeof(myAddress))<0)
		{
			throw std::runtime_error((std::string)"Could not connect to host: " + strerror(errno) + "\n");
		}
	}
}

void Socket::Close()
{
	if(m_socket >= 0) {
#if _WIN32
		closesocket(m_socket);
#else
		shutdown(m_socket,SHUT_RDWR);
		close(m_socket);
#endif
		m_socket = -1;
	}
}

void Socket::Bind(const std::string& hostname,int port,Socket::Protocol protocol)
{
	Close();
	CreateSocket(protocol);
	
	hostent *host;
	sockaddr_in myAddress;
	host = gethostbyname(hostname.c_str());
	if(!host)
	{
		throw std::runtime_error("Failed to resolve hostname.");
	}

	// prefer 192.168.x.x local addresses
	in_addr *addr = (in_addr *)host->h_addr_list[0];
#if 0
	for(int i = 0; host->h_addr_list[i]; i++) {
		if((uint8_t)host->h_addr_list[i][0] == 192 && (uint8_t)host->h_addr_list[i][1] == 168)
			addr = (in_addr *)host->h_addr_list[i];
	}
#endif
	memset((void *)&myAddress, 0, sizeof(myAddress));
	myAddress.sin_family = AF_INET;
	myAddress.sin_addr =  *addr;
	myAddress.sin_port = htons(port);
	if(bind(this->m_socket,(sockaddr *)&myAddress, sizeof(myAddress))<0)
	{	
		throw std::runtime_error("Could not bind to host.");
	}
}
	
void Socket::Listen(int backlog)
{
	if(listen(this->m_socket,backlog)<0)
	{
		throw std::runtime_error("Could not listen on host.");
	}
	
}

void Socket::Accept(Socket *outSocket)
{
	int acceptSocket = accept(this->m_socket, NULL, NULL);
	if(acceptSocket < 0) {
		throw std::runtime_error("Accept() failed.");
	}
	if(outSocket) {
		outSocket->Close();
		outSocket->m_socket = acceptSocket;
	}
}

int Socket::Read(void *buffer,int size)
{	
	int result = recv(this->m_socket, (char *)buffer, size, 0);
	if(result == 0) return -1;
	else if(result < 0) {
#if _WIN32
		switch(WSAGetLastError()) {
			case WSAEWOULDBLOCK:
#else
		switch(errno) {
			case EAGAIN: 
#endif
				return 0;
			default:
				return -1;
		}
	}
	return result;
}

int Socket::ReadFully(void *buffer,int size)
{
	int totalCharRead =0;
	int charRead =0;
	do
	{
		charRead = Read(buffer, size);
		if(charRead <= 0)
		{
			if(charRead == 0)
			{
				break;
			}
			else
			{
				if(totalCharRead) return totalCharRead;
				return -1;
			}
		}
		size -= charRead;
		(char *&) buffer += charRead;
		totalCharRead += charRead; 
	}
	while(size > 0);
	return totalCharRead;
}

int Socket::Write(const void *buffer,int size) const
{
	int result = send(this->m_socket, (char *)buffer, size,0);
	if(result == 0) return -1;
	else if(result < 0) {
#if _WIN32
		switch(WSAGetLastError()) {
			case WSAEWOULDBLOCK:
#else
		switch(errno) {
			case EAGAIN: 
#endif
				return 0;
			default:
				return -1;
		}
	}
	return result;
}

int Socket::WriteFully(const void *buffer,int size) const
{
	int totalCharWrite =0;
	do
	{
		int charWrite =0;
		charWrite = Write(buffer, size);
		if(charWrite <= 0)
		{
			if(charWrite == 0)
			{
				break;
			}
			else
			{
				if(totalCharWrite) return totalCharWrite;
				return -1;
			}
		}
		size -= charWrite;
		(char *&) buffer += charWrite;
		totalCharWrite += charWrite; 
	}
	while(size > 0);
	return totalCharWrite;	
}


int Socket::SendTo(const void *buffer,int size,IP ip,int port)
{
	struct sockaddr_in addr;
	int addrSize = sizeof(addr);

	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = ip;
	addr.sin_port = htons(port);

	int sentBytes = sendto(m_socket,(const char *)buffer,size,0,(const sockaddr *)&addr,addrSize);
	if(sentBytes < 0) {
#if _WIN32
		switch(WSAGetLastError()) {
			case WSAEWOULDBLOCK:
#else
		switch(errno) {
			case EAGAIN: 
#endif
				return 0;
			default:
				return -1;
		}
	}
	return sentBytes;
}

int Socket::RecvFrom(void *buffer,int size,IP *outIp)
{
	struct sockaddr_in addr;
#if _WIN32
	int fromLen = sizeof(addr);
#else
	socklen_t fromLen = sizeof(addr);
#endif
	
	int recvsize = recvfrom(m_socket,(char *)buffer,size,0,(sockaddr *)&addr,&fromLen);
	if(recvsize < 0) {
#if _WIN32
		switch(WSAGetLastError()) {
			case WSAEWOULDBLOCK:
#else
		switch(errno) {
			case EAGAIN: 
#endif
				return 0;
			default:
				return -1;
		}
	}
	*outIp = *(uint32_t *)&addr.sin_addr;
	return recvsize;
}

bool Socket::Poll(timeval& tv)
{
	fd_set set;
	FD_ZERO(&set);
	FD_SET(m_socket,&set);
	if(select(m_socket + 1,&set,NULL,NULL,&tv) <= 0) return false;
	return true;
}
