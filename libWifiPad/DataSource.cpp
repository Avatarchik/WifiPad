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
#ifdef TARGET_OS_IPHONE
#include <CoreGraphics/CoreGraphics.h>
#else
#include <ApplicationServices/ApplicationServices.h>
#endif
#endif
#include "DataSource.h"

namespace WifiPad
{
#if __APPLE__
	static size_t CGDataProviderGetBytes(void *info,void *buffer,size_t count)
	{
		DataSource *ds = (DataSource *)info;
		int bytes_read, total = 0;
		do {
			bytes_read = ds->ReadBytes((uint8_t *)buffer,count);
			if(bytes_read < 0) return total;
			total += bytes_read;
			count -= bytes_read;
		} while(count > 0);
		return total;
	}
	
	static void CGDataProviderSkipBytes(void *info,size_t count)
	{
		DataSource *ds = (DataSource *)ds;
		ds->SkipBytes(count);
	}
	
	static void CGDataProviderRewind(void *info)
	{
	}
	
	static void CGDataProviderReleaseInfo(void *info)
	{
	}
	
	CGDataProviderRef DataSource::CreateCGDataProvider()
	{
		CGDataProviderCallbacks callbacks;
		callbacks.getBytes = CGDataProviderGetBytes;
		callbacks.skipBytes = CGDataProviderSkipBytes;
		callbacks.rewind = CGDataProviderRewind;
		callbacks.releaseProvider = CGDataProviderReleaseInfo;
		return CGDataProviderCreate((void *)this,&callbacks);
	}
#endif
#if linux && !NO_QT
      bool DataSource::isSequential () const
      {
          return true;
      }

      qint64 DataSource::readData ( char * data, qint64 maxSize )
      {
          return ReadBytes((uint8_t *)data,maxSize);
      }

      qint64 DataSource::writeData ( const char * data, qint64 maxSize )
      {
          return -1;
      }
#endif
}
