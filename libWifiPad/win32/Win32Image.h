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
#ifndef __Win32Image_h
#define __Win32Image_h

#include <windows.h>
#include <atlbase.h>
#include <atlapp.h>
#include <atlgdi.h>
#include <vector>
#include <string>
#include <tr1/memory>
#include <inttypes.h>

namespace WifiPad
{
	class DataSource;

	class Image 
	{
	private:
		int m_width;
		int m_height;
		int m_pitch;

		uint8_t *m_pixels;
		CDC m_dc;
		CBitmap m_bitmap;
	public:
		Image() : m_width(0), m_height(0), m_pitch(0), m_pixels(NULL)
		{ 
		}
		
		Image(int width,int height);

		void Resize(int width,int height);

		// Load's a PNG from data source
		void Load(DataSource& source);

		void Blit(Image& dest,int dx = 0,int dy = 0,int width = -1,int height = -1,int sx = 0,int sy = 0) {
			::BitBlt(dest.GetDC(),dx,dy,width < 0 ? m_width : width,height < 0 ? m_height : height,m_dc,sx,sy,SRCCOPY);
		}

		operator CBitmap&() { return m_bitmap; }
		CDC& GetDC() { return m_dc; }

		int GetWidth() const { return m_width; }
		int GetHeight() const { return m_height; }
		int GetPitch() const { return m_pitch; }
	};

	typedef std::tr1::shared_ptr<Image> ImagePtr;
}

#endif
