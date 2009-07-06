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
#ifndef __QuartzRef_h
#define __QuartzRef_h

#include <CoreFoundation/CoreFoundation.h>

// This is a quartz wrapper class for automatic release
template<typename T>
class QuartzRef
{
private:
	T m_ref;

	QuartzRef& operator =(const QuartzRef&);
public:
	QuartzRef(T ref) : m_ref(ref) { }
	~QuartzRef() { if(m_ref) CFRelease(m_ref); }
	
	operator T() const { return m_ref; }
	operator bool() const { return m_ref != NULL; }
};

#endif
