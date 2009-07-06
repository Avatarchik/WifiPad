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
#include <X11/Xlib.h>
#define XK_MISCELLANY 1
#define XK_LATIN1 1
#include <X11/keysymdef.h>
#include <algorithm>
#include <ctype.h>
#include <string>
#include <sstream>
#include <map>
#include "KeySyms.h"

namespace WifiPad
{
    static std::string NormalizeCase(const std::string& str)
    {
        if(str.length() <= 1) return str;
        std::string copyOfStr(str);
        std::transform(copyOfStr.begin() + 1,copyOfStr.end(),copyOfStr.begin() + 1,tolower);
        return copyOfStr;
    }

    // maps a symbol to key, returns -1 if not found
    int KeySyms::MapSymbol(const std::string& sym)
    {
		if(sym == "SHIFT") return XK_Shift_L;
		else if(sym == "ALT") return XK_Alt_L;
		else if(sym == "CONTROL") return XK_Control_L;
		else if(sym == "SPACE") return XK_KP_Space;
		else if(sym == "BACKSPACE") return XK_BackSpace;
		else if(sym == "NUMPAD0") return XK_KP_0;
		else if(sym == "NUMPAD1") return XK_KP_1;
		else if(sym == "NUMPAD2") return XK_KP_2;
		else if(sym == "NUMPAD3") return XK_KP_3;
		else if(sym == "NUMPAD4") return XK_KP_4;
		else if(sym == "NUMPAD5") return XK_KP_5;
		else if(sym == "NUMPAD6") return XK_KP_6;
		else if(sym == "NUMPAD7") return XK_KP_7;
		else if(sym == "NUMPAD8") return XK_KP_8;
		else if(sym == "NUMPAD9") return XK_KP_9;
		else if(sym == "NUMPAD_ENTER") return XK_KP_Enter;
		else if(sym == "NUMPAD_PLUS") return XK_KP_Add;
		else if(sym == "NUMPAD_MINUS") return XK_KP_Subtract;
		else if(sym == "NUMPAD_MULTIPLY") return XK_KP_Multiply;
		else if(sym == "NUMPAD_DIVIDE") return XK_KP_Divide;

		else if(sym == "PERIOD") return XK_period;
		else if(sym == "COMMA") return XK_comma;
		else if(sym == "SLASH") return XK_slash;
		else if(sym == "SEMICOLON") return XK_semicolon;
		else if(sym == "APOSTROPHE") return XK_apostrophe;

		else if(sym == "BRACKETLEFT") return XK_bracketleft;
		else if(sym == "BRACKETRIGHT") return XK_bracketright;

        int key = XStringToKeysym(NormalizeCase(sym).c_str());
		if(key == NoSymbol) {
			if(sym.length() == 0) return -1;
			for(int i = 0; i < sym.length(); i++) {
				if(!isdigit(sym[i])) return -1;
			}
			return strtol(sym.c_str(),NULL,10);
		}
		return key;
    }

    // maps a key and returns the symbol
    std::string KeySyms::MapKey(int key)
    {
		switch(key) {
			case XK_Shift_L: return "SHIFT";
			case XK_Alt_L: return "ALT";
			case XK_Control_L: return "CONTROL";
			case XK_KP_Space: return "SPACE";
			case XK_KP_0: return "NUMPAD0";
			case XK_KP_1: return "NUMPAD1";
			case XK_KP_2: return "NUMPAD2";
			case XK_KP_3: return "NUMPAD3";
			case XK_KP_4: return "NUMPAD4";
			case XK_KP_5: return "NUMPAD5";
			case XK_KP_6: return "NUMPAD6";
			case XK_KP_7: return "NUMPAD7";
			case XK_KP_8: return "NUMPAD8";
			case XK_KP_9: return "NUMPAD9";
			case XK_KP_Enter: return "NUMPAD_ENTER";
			case XK_KP_Add: return "NUMPAD_PLUS";
			case XK_KP_Subtract: return "NUMPAD_MINUS";
			case XK_KP_Multiply: return "NUMPAD_MULTIPLY";
			case XK_KP_Divide: return "NUMPAD_DIVIDE";
		}

        const char *sym = XKeysymToString(key);
		if(!sym) {
			char i[32];
			sprintf(i,"%d",key);
			return i;
		}
        std::string copyOfStr(sym);
        std::transform(copyOfStr.begin(),copyOfStr.end(),copyOfStr.begin(),toupper);
        return copyOfStr;
    }
}
