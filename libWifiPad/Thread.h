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
#ifndef __Thread_h
#define __Thread_h

#if _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif
#include <memory>
#include <stdexcept>
#include "Mutex.h"

namespace WifiPad
{
#if _WIN32
	template<typename T>
	static DWORD WINAPI ThreadFunction(LPVOID data);
#else
	template<typename T>
	static void * ThreadFunction(void *data);
#endif
	
	
	template<typename T>
	struct ThreadInfo
	{
		T *classPtr;
		int (T::*funcPtr)(void *);
		void *data;
		
		ThreadInfo(T* c,int (T::*f)(void *),void *d) : classPtr(c), funcPtr(f), data(d) {}
	};
	
	template<typename T>
	class Thread
	{
	private:
#if _WIN32
		HANDLE m_thread;
#else
		pthread_t m_thread;
#endif
	public:
		Thread(T *classPtr,int (T::*funcPtr)(void *),void *data) 
		{
#if _WIN32
			m_thread = ::CreateThread(NULL,0,ThreadFunction<T>,new ThreadInfo<T>(classPtr,funcPtr,data),0,NULL);
			if(!m_thread) throw std::runtime_error("Failed to create thread.\n");
#else
			if(pthread_create(&m_thread,NULL,ThreadFunction<T>,new ThreadInfo<T>(classPtr,funcPtr,data)))
				throw std::runtime_error("Failed to create thread.\n");
#endif
		}

		bool Join(int *exitCode = NULL) 
		{
#if _WIN32
			if(::WaitForSingleObject(m_thread,INFINITE) == WAIT_OBJECT_0) {
				if(exitCode) ::GetExitCodeThread(m_thread,(LPDWORD)exitCode);
				::CloseHandle(m_thread);
				return true;
			}
			return false;
#else
			if(!pthread_join(m_thread,(void **)exitCode)) return true;
			return false;
#endif
		}
		
		void Detach()
		{
#if _WIN32
#else
			pthread_detach(m_thread);
#endif
		}

	};
	
#if _WIN32
	template<typename T>
	DWORD WINAPI ThreadFunction(LPVOID data)
#else
	template<typename T>
	void * ThreadFunction(void *data)
#endif
	{
		int ret;
		std::auto_ptr< ThreadInfo<T> > info((ThreadInfo<T> *)data);
		try {
			ret = ((*info->classPtr).*(info->funcPtr))(info->data);
		} catch(...) {
			// expected error here
		}
		
#if ! _WIN32
		return (void *)ret;
#else
		return ret;
#endif
	}
}

#endif
