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
#ifndef __DataSource_h
#define __DataSource_h

#include <inttypes.h>
#include <string>
#if __APPLE__
#include <TargetConditionals.h>
#if TARGET_OS_IPHONE
#include <CoreGraphics/CoreGraphics.h>
#else
#include <ApplicationServices/ApplicationServices.h>
#endif
#endif
#if linux && !NO_QT
#include <QIODevice>
#include <stdexcept>
#endif

namespace WifiPad
{
	class DataSource
#if linux && !NO_QT
            : public QIODevice
#endif
	{
	public:
		std::string name;
		
                DataSource(const std::string& name) : name(name) {
#if linux && !NO_QT
                    if(!open(QIODevice::ReadOnly)) throw std::runtime_error("Failed to open data source.\n");
#endif
                }
	
		virtual ~DataSource() {}

		// reads up to size bytes of data and store it in data
		// returns number of bytes read, or -1 on EOF
		virtual int ReadBytes(uint8_t *data,int size) = 0;

		// skips number of bytes
		virtual int SkipBytes(int size) = 0;
		
#if __APPLE__
		CGDataProviderRef CreateCGDataProvider();
#endif
#if linux && !NO_QT
                virtual bool isSequential () const;
                virtual qint64 readData ( char * data, qint64 maxSize );
                virtual qint64 writeData ( const char * data, qint64 maxSize );
#endif
	};
}

#endif
