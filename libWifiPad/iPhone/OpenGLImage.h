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
#ifndef __OpenGLImage_h
#define __OpenGLImage_h

#include <TargetConditionals.h>

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

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

		GLuint m_image;
		int m_texWidth;
		int m_texHeight;

	public:
		Image() : m_width(0), m_height(0), m_pitch(0)
		{ 
			glGenTextures(1,&m_image); 
		}
		Image(int width,int height);

		void Resize(int width,int height);

		// Load's a PNG from data source
		void Load(DataSource& source);

		~Image() { glDeleteTextures(1,&m_image); }
		void Render(int x = 0,int y = 0) const;

		int GetWidth() const { return m_width; }
		int GetHeight() const { return m_height; }
		int GetPitch() const { return m_pitch; }
	};

	typedef std::tr1::shared_ptr<Image> ImagePtr;
}

#endif
