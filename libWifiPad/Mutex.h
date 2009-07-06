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
#ifndef __Mutex_h
#define __Mutex_h

#if _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

namespace WifiPad
{
#if _WIN32
	class Mutex
	{
	private:
		CRITICAL_SECTION m_mutex;
	public:
		Mutex() { ::InitializeCriticalSection(&m_mutex); }
		Mutex(const Mutex& m) { ::InitializeCriticalSection(&m_mutex); }
		~Mutex() { ::DeleteCriticalSection(&m_mutex); }

		Mutex& operator =(const Mutex& m) { return *this; }

		void Lock() { ::EnterCriticalSection(&m_mutex); }
		void Unlock() { ::LeaveCriticalSection(&m_mutex); }
	};
#else
	class Mutex
	{
	private:
		pthread_mutex_t m_mutex;
	public:
		Mutex() { pthread_mutex_init(&m_mutex,NULL); }
                Mutex(const Mutex& ) { pthread_mutex_init(&m_mutex,NULL); }
		~Mutex() { pthread_mutex_destroy(&m_mutex); }

                Mutex& operator =(const Mutex&) { return *this; }
		
		operator pthread_mutex_t() { return m_mutex; }
		operator pthread_mutex_t *() { return &m_mutex; }

		void Lock() { pthread_mutex_lock(&m_mutex); }
		void Unlock() { pthread_mutex_unlock(&m_mutex); }
	};
#endif

	class ScopedLock
	{
	private:
		Mutex& m_mutex;
	public:
		ScopedLock(Mutex& mutex) : m_mutex(mutex) { m_mutex.Lock(); }
		~ScopedLock() { m_mutex.Unlock(); }
	};
}

#endif
