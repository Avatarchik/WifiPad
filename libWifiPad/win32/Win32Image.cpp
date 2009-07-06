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
#include <windows.h>
#include <png.h>
#include <atlbase.h>
#include <atlapp.h>
#include <atlgdi.h>
#include "DataSource.h"
#include "FileDataSource.h"
#include "Image.h"

namespace WifiPad
{
	Image::Image(int width,int height) 
	{
		Resize(width,height);
	}
	
	void Image::Resize(int width,int height)
	{
		if(!m_bitmap.IsNull()) {
			m_dc.SelectBitmap(NULL);
			m_bitmap.DeleteObject();
		}

		m_width = width;
		m_height = height;
		m_pitch = width * 4;

		BITMAPINFO info;

		ZeroMemory(&info,sizeof(info));
		info.bmiHeader.biSize = sizeof(BITMAPINFO);
		info.bmiHeader.biWidth = m_width;
		info.bmiHeader.biHeight = m_height;
		info.bmiHeader.biPlanes = 1;
		info.bmiHeader.biBitCount = 32;
		info.bmiHeader.biCompression = BI_RGB;
		m_dc.CreateCompatibleDC(NULL);
		m_bitmap.CreateDIBSection(m_dc,&info,DIB_RGB_COLORS,(void **)&m_pixels,NULL,NULL);
		m_dc.SelectBitmap(m_bitmap);
}

	// png stuff
	static void PngDataReaderFunc(png_structp pngPtr,png_bytep data,png_size_t length)
	{
		DataSource *source = (DataSource *)png_get_io_ptr(pngPtr);
		int size = source->ReadBytes(data,length);
		if(size != length) png_error(pngPtr,"Unexpected end-of-file.");
	}

	void Image::Load(DataSource& source)
	{
		struct PngReadStruct
		{
			png_structp pngPtr;
			png_infop infoPtr;
			png_infop endInfo;
			
			PngReadStruct() 
			{
				// create png read struct
				this->pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING,(png_voidp)NULL,NULL,NULL);
				if(!this->pngPtr) throw std::runtime_error("Failed to create read struct.\n");
				
				// create info struct
				this->infoPtr = png_create_info_struct(this->pngPtr);
				if(!this->infoPtr) {
					png_destroy_read_struct(&this->pngPtr,0,0);
					throw std::runtime_error("Failed to create info struct.\n");
				}
				
				this->endInfo = png_create_info_struct(this->pngPtr);
				if(!this->endInfo) {
					png_destroy_read_struct(&this->pngPtr,&this->infoPtr,0);
					throw std::runtime_error("Failed to create info struct.\n");
				}
			}

			~PngReadStruct() {
				png_destroy_read_struct(&this->pngPtr,&this->infoPtr,&this->endInfo);
			}
		};

		unsigned char header[8];
		int is_png;
		png_bytep row_pointer;
		unsigned long width, height;
		int bit_depth, color_type, interlace_type, compression_type, filter_method;
		unsigned int  channels;
		double gamma, screen_gamma;
		char *gamma_str;

		// check png header
		source.ReadBytes(header,8);
		is_png = !png_sig_cmp(header,0,8);
		if(!is_png) throw std::runtime_error("File is not in PNG format.\n");

		PngReadStruct info;

		// set up input io
		png_set_read_fn(info.pngPtr,(voidp)&source,PngDataReaderFunc);
		png_set_sig_bytes(info.pngPtr,8);

		// read info
		png_read_info(info.pngPtr,info.infoPtr);
		png_get_IHDR(info.pngPtr,info.infoPtr,&width,&height,
			 &bit_depth,&color_type,&interlace_type,
			 &compression_type,&filter_method);

		/* perform input transformations */

		// change to RGB
		if (color_type == PNG_COLOR_TYPE_PALETTE)
			png_set_palette_to_rgb(info.pngPtr);

		// only use 8-bit per channel
		if (bit_depth == 16)
			png_set_strip_16(info.pngPtr);

		// we want bgr
		if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_RGB_ALPHA)
		   png_set_bgr(info.pngPtr);

		// fill alpha
		if (color_type == PNG_COLOR_TYPE_RGB)
			png_set_filler(info.pngPtr,0xFF,PNG_FILLER_AFTER);

		// convert grayscale to RGB
		if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
			png_set_gray_to_rgb(info.pngPtr);
			png_set_filler(info.pngPtr,0xFF,PNG_FILLER_AFTER);
		}

		// can use SCREEN_GAMMA environment variable to do gamma correction
		gamma_str = getenv("SCREEN_GAMMA");
		if(gamma_str) screen_gamma = (double)atof(gamma_str);
		else screen_gamma = 2.2; // use default 2.2 screen gamma

		if (png_get_gAMA(info.pngPtr,info.infoPtr,&gamma))
			png_set_gamma(info.pngPtr,screen_gamma,gamma); /* FIXME: Get correct value for gamma */

		// get number of channels
		channels = png_get_channels(info.pngPtr,info.infoPtr);

		Resize(width,height);

		int i = height - 1;
		while(i >= 0) {
			row_pointer = (unsigned char *)m_pixels + i * m_pitch;
			png_read_row(info.pngPtr,row_pointer,NULL);
			i--;
		}
		png_read_end(info.pngPtr,info.endInfo);
	}
}
