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
#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <map>
#include <ctype.h>
#include <assert.h>
#include <string.h>
#include <inttypes.h>
#include "Uuid.h"
#include "DataSource.h"
#include "Image.h"
#include "Ar.h"
#include "LimitReader.h"
#include "BufferedInputReader.h"
#include "KeySyms.h"
#include "GamePad.h"

namespace WifiPad
{
	// trims a trim, returns pointer into the string from the left trimmed portion
	static char *Trim(char *string)
	{
		char *trimmed = string;
		while(isspace(*trimmed)) trimmed++;
		int n = strlen(trimmed);
		while(--n >= 0 && isspace(trimmed[n])) trimmed[n] = 0;
		return trimmed;
	}


	static bool IsNumeric(const char *string)
	{
		assert(string);
		
		if(!*string) return false;
		while(*string) {
			if(!isdigit(*string)) return false;
			string++;
		}
		return true;
	}
	
	void GamePad::Clear()
	{
		ScopedLock lock(m_lock);
		m_name = "";
		m_gamepadImage.reset();
		std::vector<ButtonInfo>().swap(m_buttons);
	}

	void GamePad::LoadGamePad(DataSource& source,int loadLevel)
	{
		ScopedLock lock(m_loadLock);

		// clear data
		Clear();

		char globalHeader[8];
		BufferedInputReader reader(source);
		
		// read global header
		reader.ReadBytes((uint8_t *)globalHeader,8);
		if(strncmp(globalHeader,"!<arch>\n",8) != 0)
			throw std::runtime_error(source.name + ": Not a valid gamepad.\n");

		ArFileHeader header;
		
		header.ReadHeader(reader);
		if(!header.VerifyMagic()) 
			throw std::runtime_error(source.name + ": Invalid/corrupted gamepad file.\n");

		// parse the configuration file
		if(strncmp(header.filename,"gamepad.txt",11))
			throw std::runtime_error(source.name + ": Invalid/corrupted gamepad file.\n");
		
		// read remaining byte
		int remainingBytes = header.GetFileSize();
		remainingBytes += remainingBytes & 1; // padding

		char line[1024];
		int lineCount = 0;
		std::stringstream errStr;
		std::multimap<const std::string,int> imageMap; // maps images to button number for delayed image loading
		uint32_t gp_version = 0;
		bool uuidFound = false;
		bool nameFound = false;
		
		while(remainingBytes > 0) {
			++lineCount;
			errStr.str("");
			errStr << "Error loading gamepad \"" << source.name << "\": Parse error gamepad.txt on line " << lineCount << ": ";

			int readBytes = reader.ReadLine((uint8_t *)line,remainingBytes + 1 > 1024 ? 1024 : remainingBytes + 1);
			if(readBytes == -1) break;
			remainingBytes -= readBytes;

			// split by :
			char *key = strtok(line,":");
			if(!key) continue;
			char *value = strtok(NULL,"");
			if(!value) continue;
			value = Trim(value);

			// check for Tag: GP10 or GP11
			if(!gp_version) {
				if(strcmp(key,"Tag") == 0) {
					if(strcmp(value,"GP10") == 0) gp_version = 0x01000000;
					else if(strcmp(value,"GP11") == 0) gp_version = 0x01010000;
				}
				if(!gp_version) {
					errStr << "Invalid gamepad format. Must contain Tag: GP10/GP11 as first entry.\n";
					throw std::runtime_error(errStr.str());
				}
				continue;
			}

			// read no more if load level is 0
			if(strcmp(key,"Name") == 0) {
				m_name = value;
				nameFound = true;
			} else if(strcmp(key,"UUID") == 0) {
				m_uuid.ParseString(value);
				uuidFound = true;
			} else if(strcmp(key,"Image") == 0) {
				imageMap.insert(std::pair<const std::string,int>(value,-1));
			} else if(strcmp(key,"NumButtons") == 0) {
				if(!IsNumeric(value)) {
					errStr << "Expects integer.\n";
					throw std::runtime_error(errStr.str());
				}
				int numButtons = strtol(value,NULL,0);
				if(numButtons > 256) numButtons = 256;
				m_buttons.reserve(numButtons);
			} else if(strcmp(key,"Button") == 0 || (gp_version >= 0x01010000 && strcmp(key,"Trackpad") == 0)) {
				char *rect = strtok(value,";");
				char *imageInfo = strtok(NULL,";");
				char *defaultKey = strtok(NULL,";");

				if(!rect) {
					errStr << "Invalid format for Button. Expecting ';'.\n";
					throw std::runtime_error(errStr.str());
				}
				
				rect = Trim(rect);

				ButtonInfo button;
				if(strcmp(key,"Trackpad") == 0) button.isTrackPad = true;

				// tokenize the rectangles
				char *coordinate = strtok(rect,",");
				if(!coordinate) {
					errStr << "Expecting 4 rectangle coordinates.\n";
					throw std::runtime_error(errStr.str());
				}
				coordinate = Trim(coordinate);
				if(!IsNumeric(coordinate)) {
					errStr << "Expecting integer.\n";
					throw std::runtime_error(errStr.str());
				}
				button.x = strtol(coordinate,NULL,0);

				coordinate = strtok(NULL,",");
				if(!coordinate) {
					errStr << "Expecting 4 rectangle coordinates.\n";
					throw std::runtime_error(errStr.str());
				}
				coordinate = Trim(coordinate);
				if(!IsNumeric(coordinate)) {
					errStr << "Expecting integer.\n";
					throw std::runtime_error(errStr.str());
				}
				button.y = strtol(coordinate,NULL,0);
				
				coordinate = strtok(NULL,",");
				if(!coordinate) {
					errStr << "Expecting 4 rectangle coordinates.\n";
					throw std::runtime_error(errStr.str());
				}
				coordinate = Trim(coordinate);
				if(!IsNumeric(coordinate)) {
					errStr << "Expecting integer.\n";
					throw std::runtime_error(errStr.str());
				}
				button.width = strtol(coordinate,NULL,0);

				coordinate = strtok(NULL,",");
				if(!coordinate) {
					errStr << "Expecting 4 rectangle coordinates.\n";
					throw std::runtime_error(errStr.str());
				}
				coordinate = Trim(coordinate);
				if(!IsNumeric(coordinate)) {
					errStr << "Expecting integer.\n";
					throw std::runtime_error(errStr.str());
				}
				button.height = strtol(coordinate,NULL,0);
				
				if(button.x < 0) {
					errStr << "Rectangle X1 cannot be negative.\n";
					throw std::runtime_error(errStr.str());
				}
				if(button.y < 0) {
					errStr << "Rectangle Y1 cannot be negative.\n";
					throw std::runtime_error(errStr.str());
				}
				if(button.width > 480) {
					errStr << "Rectangle X2 cannot be > 480.\n";
					throw std::runtime_error(errStr.str());
				}
				if(button.height > 320) {
					errStr << "Rectangle Y2 cannot be > 320.\n";
					throw std::runtime_error(errStr.str());
				}
				if(button.width <= button.x){
					errStr << "Rectangle X2 <= X1.\n";
					throw std::runtime_error(errStr.str());
				}
				if(button.height <= button.y) {
					errStr << "Rectangle Y2 <= Y1.\n";
					throw std::runtime_error(errStr.str());
				}

				if(button.width > 480) {
					errStr << "Rectangle X2 > 480.\n";
					throw std::runtime_error(errStr.str());
				}
				
				if(button.height > 320) {
					errStr << "Rectangle Y2 > 320.\n";
					throw std::runtime_error(errStr.str());
				}
				
				button.width -= button.x;
				button.height -= button.y;
				
				// image info if availables
				if(imageInfo) {
					imageInfo = Trim(imageInfo);
					if(*imageInfo) {
						char *imageFilename = strtok(imageInfo,",");
						if(!imageFilename) continue;
						imageFilename = Trim(imageFilename);
						imageMap.insert(std::pair<const std::string,int>(imageFilename,GetNumButtons()));

						char *imageX = strtok(NULL,",");
						if(!imageX) button.imageX = 0;
						else {
							imageX = Trim(imageX);
							if(!IsNumeric(imageX)) {
								errStr << "Expecting integer.\n";
								throw std::runtime_error(errStr.str());
							}
							button.imageX = strtol(imageX,NULL,0);
							if(button.imageX < 0) {
								errStr << "Image X cannot be negative.\n";
								throw std::runtime_error(errStr.str());
							}
						}

						char *imageY = strtok(NULL,",");
						if(!imageY) button.imageY = 0;
						else {
							imageY = Trim(imageY);
							if(!IsNumeric(imageY)) {
								errStr << "Expecting integer.\n";
								throw std::runtime_error(errStr.str());
							}
							button.imageY = strtol(imageY,NULL,0);
							if(button.imageY < 0) {
								errStr << "Image Y cannot be negative.\n";
								throw std::runtime_error(errStr.str());
							}
						}
					}
				}
				
				if(defaultKey) {
					char *key = strtok(defaultKey,",");
					for(int i = 0; key && i < 3; i++) {
						key = Trim(key);
						button.defaultKey[i] = KeySyms::MapSymbol(key);
						key = strtok(NULL,",");
					}
				}

				AddButton(button);
			} else {
				errStr << "Unrecognized key '" << key << "'.\n";
				throw std::runtime_error(errStr.str());
			}
		}

		// locate images
		for( ; loadLevel >= 1; ) {
			// read 15-char filename
			if(!header.ReadHeader(reader)) break;

			if(!header.VerifyMagic()) 
				throw std::runtime_error(source.name + ": Invalid/corrupted gamepad file.\n");

			// check if image is references
			std::pair<std::multimap<const std::string,int>::const_iterator,
				std::multimap<const std::string,int>::const_iterator> imageRefs;
			
			const int fileSize = header.GetFileSize();
			if(imageMap.find(header.filename) != imageMap.end()) {
				LimitReader limit(reader,fileSize + (fileSize & 1));
				imageRefs = imageMap.equal_range(header.filename);
				
				ImagePtr image(new Image());
				image->Load(limit);
				for(std::multimap<const std::string,int>::const_iterator 
					it = imageRefs.first; it != imageRefs.second; ++it) {
					if(it->second == -1) {
						if(image->GetWidth() != 480 || image->GetHeight() != 320) {
							throw std::runtime_error(source.name + ": Invalid/corrupted gamepad file. Main image is not 480x320!\n");
						}
						m_gamepadImage = image;
					} else if(it->second < GetNumButtons()){
						ButtonInfo& button = GetButton(it->second);
						if(image->GetWidth() > 480 || image->GetHeight() > 320) {
							throw std::runtime_error(source.name + ": Invalid/corrupted gamepad file. All images must be < 480x320 in size.\n");
						}
						button.image = image;
					}
				}
			} else {
				reader.SkipBytes(fileSize + (fileSize & 1));
			}
		}
	}
}
