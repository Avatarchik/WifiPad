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
#include <map>
#include "KeySyms.h"

namespace WifiPad
{
	std::map<int,std::string> KeySyms::m_keyToKeySym;
	std::map<const std::string,int> KeySyms::m_keySymToKey;
	
	void KeySyms::Initialize()
	{
		static bool initFlag = false;

		if(!initFlag) {
#define MAP_KEYSYM(symbol,keycode)  m_keyToKeySym[keycode] = #symbol; m_keySymToKey[#symbol] = keycode;
			MAP_KEYSYM(A,0)
			MAP_KEYSYM(S,1)
			MAP_KEYSYM(D,2)
			MAP_KEYSYM(F,3)
			MAP_KEYSYM(H,4)
			MAP_KEYSYM(G,5)
			MAP_KEYSYM(Z,6)
			MAP_KEYSYM(X,7)
			MAP_KEYSYM(C,8)
			MAP_KEYSYM(V,9)
			MAP_KEYSYM(B,11)
			MAP_KEYSYM(Q,12)
			MAP_KEYSYM(W,13)
			MAP_KEYSYM(E,14)
			MAP_KEYSYM(R,15)
			MAP_KEYSYM(Y,16)
			MAP_KEYSYM(T,17)
			MAP_KEYSYM(1,18)
			MAP_KEYSYM(2,19)
			MAP_KEYSYM(3,20)
			MAP_KEYSYM(4,21)
			MAP_KEYSYM(6,22)
			MAP_KEYSYM(5,23)
			MAP_KEYSYM(EQUALS,24)
			MAP_KEYSYM(9,25)
			MAP_KEYSYM(7,26)
			MAP_KEYSYM(MINUS,27)
			MAP_KEYSYM(8,28)
			MAP_KEYSYM(0,29)
			MAP_KEYSYM(RIGHTBRACKET,30)
			MAP_KEYSYM(O,31)
			MAP_KEYSYM(U,32)
			MAP_KEYSYM(LEFTBRACKET,33)
			MAP_KEYSYM(I,34)
			MAP_KEYSYM(P,35)
			MAP_KEYSYM(RETURN,36)
			MAP_KEYSYM(L,37)
			MAP_KEYSYM(J,38)
			MAP_KEYSYM(QUOTE,39)
			MAP_KEYSYM(K,40)
			MAP_KEYSYM(SEMICOLON,41)
			MAP_KEYSYM(BACKSLASH,42)
			MAP_KEYSYM(COMMA,43)
			MAP_KEYSYM(SLASH,44)
			MAP_KEYSYM(N,45)
			MAP_KEYSYM(M,46)
			MAP_KEYSYM(PERIOD,47)
			MAP_KEYSYM(TAB,48)
			MAP_KEYSYM(SPACE,49)
			MAP_KEYSYM(BACKQUOTE,50)
			MAP_KEYSYM(BACKSPACE,51)
			MAP_KEYSYM(ESCAPE,53)
			MAP_KEYSYM(RMETA,54)
			MAP_KEYSYM(LMETA,55)
			MAP_KEYSYM(META,55)
			MAP_KEYSYM(LSHIFT,56)
			MAP_KEYSYM(SHIFT,56)
			MAP_KEYSYM(CAPSLOCK,57)
			MAP_KEYSYM(LALT,58)
			MAP_KEYSYM(ALT,58)
			MAP_KEYSYM(LCONTROL,59)
			MAP_KEYSYM(CONTROL,59)
			MAP_KEYSYM(RSHIFT,60)
			MAP_KEYSYM(RALT,61)
			MAP_KEYSYM(RCONTROL,62)
			MAP_KEYSYM(NUMPAD_PERIOD,65)
			MAP_KEYSYM(NUMPAD_MULTIPLY,67)
			MAP_KEYSYM(NUMPAD_PLUS,69)
			MAP_KEYSYM(NUMLOCK,71)
			MAP_KEYSYM(NUMPAD_DIVIDE,75)
			MAP_KEYSYM(NUMPAD_ENTER,76)
			MAP_KEYSYM(NUMPAD_MINUS,78)
			MAP_KEYSYM(NUMPAD_EQUALS,81)
			MAP_KEYSYM(NUMPAD0,82)
			MAP_KEYSYM(NUMPAD1,83)
			MAP_KEYSYM(NUMPAD2,84)
			MAP_KEYSYM(NUMPAD3,85)
			MAP_KEYSYM(NUMPAD4,86)
			MAP_KEYSYM(NUMPAD5,87)
			MAP_KEYSYM(NUMPAD6,88)
			MAP_KEYSYM(NUMPAD7,89)
			MAP_KEYSYM(NUMPAD8,91)
			MAP_KEYSYM(NUMPAD9,92)
			MAP_KEYSYM(F5,96)
			MAP_KEYSYM(F6,97)
			MAP_KEYSYM(F7,98)
			MAP_KEYSYM(F3,99)
			MAP_KEYSYM(F8,100)
			MAP_KEYSYM(F9,101)
			MAP_KEYSYM(F11,103)
			MAP_KEYSYM(SCROLLOCK,107)
			MAP_KEYSYM(F10,109)
			MAP_KEYSYM(F12,111)
			MAP_KEYSYM(PAUSE,113)
			MAP_KEYSYM(INSERT,114)
			MAP_KEYSYM(HOME,115)
			MAP_KEYSYM(PAGEUP,116)
			MAP_KEYSYM(DELETE,117)
			MAP_KEYSYM(F4,118)
			MAP_KEYSYM(END,119)
			MAP_KEYSYM(F2,120)
			MAP_KEYSYM(PAGEDOWN,121)
			MAP_KEYSYM(F1,122)
			MAP_KEYSYM(LEFT,123)
			MAP_KEYSYM(RIGHT,124)
			MAP_KEYSYM(DOWN,125)
			MAP_KEYSYM(UP,126)		
			
			MAP_KEYSYM(LBUTTON,1000)
			MAP_KEYSYM(RBUTTON,1001)
			MAP_KEYSYM(MBUTTON,1002)
			MAP_KEYSYM(MOUSEUP,1003)
			MAP_KEYSYM(MOUSEDOWN,1004)
			MAP_KEYSYM(MOUSELEFT,1005)
			MAP_KEYSYM(MOUSERIGHT,1006)
			MAP_KEYSYM(WHEELUP,1007)
			MAP_KEYSYM(WHEELDOWN,1008)

			initFlag = true;
		}
	}

	// maps a symbol to key, returns -1 if not found
	int KeySyms::MapSymbol(const std::string& sym)
	{
		Initialize();
		std::map<const std::string,int>::const_iterator it;

		it = m_keySymToKey.find(sym);
		if(it == m_keySymToKey.end()) return -1;
		return it->second;
	}

	// maps a key and returns the symbol
	std::string KeySyms::MapKey(int key)
	{
		Initialize();
		std::map<int,std::string>::const_iterator it;

		if(key == -1) return "";

		it = m_keyToKeySym.find(key);
		if(it == m_keyToKeySym.end()) {
			std::stringstream ss;
			ss << key;
			return ss.str();
		}
		return it->second;
	}
}
