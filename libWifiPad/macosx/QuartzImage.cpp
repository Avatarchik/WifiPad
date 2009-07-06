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
#include <stdexcept>
#include <inttypes.h>
#include <CoreFoundation/CoreFoundation.h>
#include <ApplicationServices/ApplicationServices.h>
#include "QuartzRef.h"
#include "DataSource.h"
#include "FileDataSource.h"
#include "QuartzImage.h"

namespace WifiPad
{
	Image::Image(int width,int height) 
	{
		Resize(width,height);
	}
	
	void Image::Resize(int width,int height)
	{
	}
		
	void Image::Load(DataSource& source)
	{
		// create a data provider based on File
		QuartzRef<CGDataProviderRef> data_provider = source.CreateCGDataProvider();
		if(!data_provider) throw std::runtime_error("Could not load image.\n");
		
		// load image
		m_image = CGImageCreateWithPNGDataProvider(data_provider,NULL,FALSE,kCGRenderingIntentDefault);
		if(!m_image) throw std::runtime_error("Could not load image.\n");
	}
}
