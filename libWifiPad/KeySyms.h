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
#ifndef __KeySyms_h
#define __KeySyms_h

#include <map>
#include <string>

namespace WifiPad
{
	class KeySyms
	{
	private:
		static std::map<int,std::string> m_keyToKeySym;
		static std::map<const std::string,int> m_keySymToKey;
	public:
		enum SpecialSymbols
		{
			MOUSEEVENTS = 1000,
			LBUTTON = 1000,
			RBUTTON = 1001,
			MBUTTON = 1002,
			MOUSEUP = 1003,
			MOUSEDOWN = 1004,
			MOUSELEFT = 1005,
			MOUSERIGHT = 1006,
			WHEELUP = 1007,
			WHEELDOWN = 1008
		};
		
		static void Initialize();

		// maps a symbol to key, returns 0 if not found
		static int MapSymbol(const std::string& sym);

		// maps a key and returns the symbol
		static std::string MapKey(int key);
	};
}

#endif
