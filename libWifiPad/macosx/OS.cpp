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

#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <CoreFoundation/CoreFoundation.h>
#include <mach-o/dyld.h>
#include "QuartzRef.h"
#include <stdexcept>
#include <string>
#include "OS.h"

namespace WifiPad
{
	namespace OS 
	{
		std::string GetPathToExe()
		{
			char buffer[4096];
			uint32_t pathSize = 4096;
			// use NSGetExecutablePath on apple
			if(_NSGetExecutablePath(buffer,&pathSize) < 0)
				throw std::runtime_error("Could not determine path to executable.");
			buffer[pathSize] = 0;
			return buffer;
		}
		
		std::string GetResourcesPath()
		{

			char buffer[1024];
			// Get resource path on apple
			CFBundleRef bundle = CFBundleGetMainBundle();
			if(bundle) {
				QuartzRef<CFURLRef> res_url = CFBundleCopyResourcesDirectoryURL(bundle);
				if(res_url) {
					if(CFURLGetFileSystemRepresentation(res_url,TRUE,(UInt8 *)buffer,1023)) {
						buffer[1023] = 0;
						return buffer;
					}
				}
			}
			throw std::runtime_error("Failed to get resources path.\n");
		}
		
		std::string GetAppDataPath()
		{
			char *home = getenv("HOME");
			if(home == NULL) throw std::runtime_error("Could not determine user's HOME directory.");
			std::string appDataPath = home;
			appDataPath += "/Library/Application Support/WifiPadServer";
			return appDataPath;
		}

		void CreateDirectory(const std::string& path,int mode)
		{
			mkdir(path.c_str(),mode);
		}
		
		void DeleteDirectory(const std::string& path)
		{
			rmdir(path.c_str());
		}
		
		void DeleteFile(const std::string& path)
		{
			unlink(path.c_str());
		}
		
		void CopyFile(const std::string& srcPath,const std::string& destPath)
		{
			FILE *srcfp = fopen(srcPath.c_str(),"rb");
			if(!srcfp) throw std::runtime_error(srcPath + ": File not found.");
			FILE *destfp = fopen(destPath.c_str(),"wb");
			if(!destfp) {
				fclose(srcfp);
				throw std::runtime_error(destPath + ": File could not be opened for writing.");
			}
			
			char buffer[1024];
			int n;
			while((n = fread(buffer,1,1024,srcfp)) > 0) {
				fwrite(buffer,1,n,destfp);
			}
			fclose(srcfp);
			fclose(destfp);
		}

		bool FileExists(const std::string& path)
		{
			FILE *fp = fopen(path.c_str(),"r");
			if(!fp) return false;
			fclose(fp);
			return true;
		}
	}
}
