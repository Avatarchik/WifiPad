/*
	This file is part of WifiPad.

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
#include <stdexcept>
#include <inttypes.h>
#include <CoreFoundation/CoreFoundation.h>
#include <TargetConditionals.h>
#include <CoreGraphics/CoreGraphics.h>
#include "QuartzRef.h"
#include "DataSource.h"
#include "FileDataSource.h"
#include "Image.h"

namespace WifiPad
{
	Image::Image(int width,int height) 
	{
		glGenTextures(1,&m_image);
		Resize(width,height);
	}

	static inline bool IsPowerOfTwo(unsigned int num)
	{
		return (num > 0) && (num & (num - 1)) == 0; 
	}
	
	static unsigned int RoundUpToPowerOfTwo(unsigned int num)
	{
		if(IsPowerOfTwo(num)) return num;
		for(int i = sizeof(unsigned int) * 8 - 1; i >= 0; i--)
			if(num & (1 << i)) return 1 << (i + 1);
		return 1;
	}
	
	void Image::Resize(int width,int height)
	{
		m_width = width;
		m_height = height;
		m_pitch = width * 4;

		m_texWidth = RoundUpToPowerOfTwo(width);
		m_texHeight = RoundUpToPowerOfTwo(height);
		
		GLint oldTex;
		glGetIntegerv(GL_TEXTURE_BINDING_2D,&oldTex);
		glBindTexture(GL_TEXTURE_2D,m_image);
		
		glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
		
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,m_texWidth,m_texHeight,0,GL_BGRA,GL_UNSIGNED_BYTE,NULL);
		
		glBindTexture(GL_TEXTURE_2D,oldTex);
	}

	void Image::Load(DataSource& source)
	{
		// create a data provider based on File
		QuartzRef<CGDataProviderRef> data_provider = source.CreateCGDataProvider();
		if(!data_provider) throw std::runtime_error("Could not load image.\n");
		
		// load image
		QuartzRef<CGImageRef> image = CGImageCreateWithPNGDataProvider(data_provider,NULL,FALSE,kCGRenderingIntentDefault);
		if(!image) throw std::runtime_error("Could not load image.\n");

		const int width = CGImageGetWidth(image);
		const int height = CGImageGetHeight(image);
		
		Resize(width,height);
		
		std::vector<uint8_t> pixels((m_texWidth << 2) * m_texHeight);
		
		QuartzRef<CGColorSpaceRef> color_space = CGColorSpaceCreateDeviceRGB();
		if(!color_space) throw std::runtime_error("Could not load image.\n");
		
		QuartzRef<CGContextRef> bitmap = CGBitmapContextCreate(&pixels[0],width,height,8,(m_texWidth << 2),color_space,kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Little);
		if(!bitmap) throw std::runtime_error("Could not load image.\n");
		
		CGContextClearRect(bitmap,CGRectMake(0.0,0.0,width,height));
		CGContextDrawImage(bitmap,CGRectMake(0.0f,0.0f,width,height),image);

		uint8_t *a = (uint8_t *)&pixels[0];
		for(int i = 0; i < m_texWidth * height; i++) {
			a[0] = (a[0] * a[3]) >> 8;
			a[1] = (a[1] * a[3]) >> 8;
			a[2] = (a[2] * a[3]) >> 8;
			a += 4;
		}
		
		GLint oldTex;
		glGetIntegerv(GL_TEXTURE_BINDING_2D,&oldTex);
		
		glBindTexture(GL_TEXTURE_2D,m_image);
		glTexSubImage2D(GL_TEXTURE_2D,0,0,0,m_texWidth,m_texHeight,GL_BGRA,GL_UNSIGNED_BYTE,&pixels[0]);
		
		glBindTexture(GL_TEXTURE_2D,oldTex);
	}
	
	void Image::Render(int x,int y) const
	{
			
		const int x2 = x + m_width;
		const int y2 = y + m_height;
		const float rect[] = { 
			x, y,
			x, y2,
			x2, y,
			x2, y2 };
		
		const float uv[] = {
			0.0f, 0.0f,
			0.0f, (float)m_height / m_texHeight,
			(float)m_width / m_texWidth, 0.0f,
			(float)m_width / m_texWidth, (float)m_height / m_texHeight
		};
	
		GLint oldTex;
		glGetIntegerv(GL_TEXTURE_BINDING_2D,&oldTex);
		glBindTexture(GL_TEXTURE_2D,m_image);
		
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(2,GL_FLOAT,0,rect);
		glTexCoordPointer(2,GL_FLOAT,0,uv);
		glDrawArrays(GL_TRIANGLE_STRIP,0,4);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		
		glBindTexture(GL_TEXTURE_2D,oldTex);
	}

}
