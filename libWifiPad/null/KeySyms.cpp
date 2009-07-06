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
#include "KeySyms.h"

namespace WifiPad
{
	// maps a symbol to key, returns -1 if not found
	int KeySyms::MapSymbol(const std::string& sym)
	{
		return -1;
	}

	// maps a key and returns the symbol
	std::string KeySyms::MapKey(int key)
	{
		return "";
	}
}
