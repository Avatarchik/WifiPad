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
#ifndef __QuartzImage_h
#define __QuartzImage_h

#include <CoreFoundation/CoreFoundation.h>
#include <ApplicationServices/ApplicationServices.h>
#include <boost/shared_ptr.hpp>

namespace WifiPad
{
	class DataSource;
	class Image 
	{
	private:
		CGImageRef m_image;
	public:
		Image() : m_image(NULL) { }
		~Image() { CGImageRelease(m_image); }

		Image(int width,int height);
		
		operator CGImageRef() { return m_image; }
		
		void Resize(int width,int height);
		
		// Load's a PNG from data source
		void Load(DataSource& source);
		int GetWidth() const { return CGImageGetWidth(m_image); }
		int GetHeight() const { return CGImageGetHeight(m_image); }
		int GetPitch() const { return CGImageGetBytesPerRow(m_image); }
	};
	
	typedef boost::shared_ptr<Image> ImagePtr;
}

#endif
