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
#include <windows.h>
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
#define DEFINE_KEYSYM(key) m_keyToKeySym[VK_##key] = #key; m_keySymToKey[#key] = VK_##key;
			DEFINE_KEYSYM(CANCEL)
			DEFINE_KEYSYM(BACK)
			DEFINE_KEYSYM(TAB)
			DEFINE_KEYSYM(CLEAR)
			DEFINE_KEYSYM(RETURN)
			DEFINE_KEYSYM(SHIFT)
			DEFINE_KEYSYM(CONTROL)
			DEFINE_KEYSYM(MENU)
			DEFINE_KEYSYM(PAUSE)
			DEFINE_KEYSYM(CAPITAL)
			DEFINE_KEYSYM(KANA)
			DEFINE_KEYSYM(HANGEUL)
			DEFINE_KEYSYM(HANGUL)
			DEFINE_KEYSYM(JUNJA)
			DEFINE_KEYSYM(FINAL)
			DEFINE_KEYSYM(HANJA)
			DEFINE_KEYSYM(KANJI)
			DEFINE_KEYSYM(ESCAPE)
			DEFINE_KEYSYM(CONVERT)
			DEFINE_KEYSYM(NONCONVERT)
			DEFINE_KEYSYM(ACCEPT)
			DEFINE_KEYSYM(MODECHANGE)
			DEFINE_KEYSYM(SPACE)
			DEFINE_KEYSYM(PRIOR)
			DEFINE_KEYSYM(NEXT)
			DEFINE_KEYSYM(END)
			DEFINE_KEYSYM(HOME)
			DEFINE_KEYSYM(LEFT)
			DEFINE_KEYSYM(UP)
			DEFINE_KEYSYM(RIGHT)
			DEFINE_KEYSYM(DOWN)
			DEFINE_KEYSYM(SELECT)
			DEFINE_KEYSYM(PRINT)
			DEFINE_KEYSYM(EXECUTE)
			DEFINE_KEYSYM(SNAPSHOT)
			DEFINE_KEYSYM(INSERT)
			DEFINE_KEYSYM(DELETE)
			DEFINE_KEYSYM(HELP)


			int i;
			for(i = '0'; i <= '9'; i++) {
				char str[2] = { 0, 0 };
				str[0] = i;
				m_keyToKeySym[i] = str; m_keySymToKey[str] = i;
			}

			for(i = 'A'; i <= 'Z'; i++) {
				char str[2] = { 0, 0 };
				str[0] = i;
				m_keyToKeySym[i] = str; m_keySymToKey[str] = i;
			}

			DEFINE_KEYSYM(LWIN)
			DEFINE_KEYSYM(RWIN)
			DEFINE_KEYSYM(APPS)
			DEFINE_KEYSYM(SLEEP)
			DEFINE_KEYSYM(NUMPAD0)
			DEFINE_KEYSYM(NUMPAD1)
			DEFINE_KEYSYM(NUMPAD2)
			DEFINE_KEYSYM(NUMPAD3)
			DEFINE_KEYSYM(NUMPAD4)
			DEFINE_KEYSYM(NUMPAD5)
			DEFINE_KEYSYM(NUMPAD6)
			DEFINE_KEYSYM(NUMPAD7)
			DEFINE_KEYSYM(NUMPAD8)
			DEFINE_KEYSYM(NUMPAD9)
			DEFINE_KEYSYM(MULTIPLY)
			DEFINE_KEYSYM(ADD)
			DEFINE_KEYSYM(SEPARATOR)
			DEFINE_KEYSYM(SUBTRACT)
			DEFINE_KEYSYM(DECIMAL)
			DEFINE_KEYSYM(DIVIDE)
			DEFINE_KEYSYM(F1)
			DEFINE_KEYSYM(F2)
			DEFINE_KEYSYM(F3)
			DEFINE_KEYSYM(F4)
			DEFINE_KEYSYM(F5)
			DEFINE_KEYSYM(F6)
			DEFINE_KEYSYM(F7)
			DEFINE_KEYSYM(F8)
			DEFINE_KEYSYM(F9)
			DEFINE_KEYSYM(F10)
			DEFINE_KEYSYM(F11)
			DEFINE_KEYSYM(F12)
			DEFINE_KEYSYM(F13)
			DEFINE_KEYSYM(F14)
			DEFINE_KEYSYM(F15)
			DEFINE_KEYSYM(F16)
			DEFINE_KEYSYM(F17)
			DEFINE_KEYSYM(F18)
			DEFINE_KEYSYM(F19)
			DEFINE_KEYSYM(F20)
			DEFINE_KEYSYM(F21)
			DEFINE_KEYSYM(F22)
			DEFINE_KEYSYM(F23)
			DEFINE_KEYSYM(F24)
			DEFINE_KEYSYM(NUMLOCK)
			DEFINE_KEYSYM(SCROLL)
			DEFINE_KEYSYM(OEM_NEC_EQUAL)
			DEFINE_KEYSYM(OEM_FJ_JISHO)
			DEFINE_KEYSYM(OEM_FJ_MASSHOU)
			DEFINE_KEYSYM(OEM_FJ_TOUROKU)
			DEFINE_KEYSYM(OEM_FJ_LOYA)
			DEFINE_KEYSYM(OEM_FJ_ROYA)
			DEFINE_KEYSYM(LSHIFT)
			DEFINE_KEYSYM(RSHIFT)
			DEFINE_KEYSYM(LCONTROL)
			DEFINE_KEYSYM(RCONTROL)
			DEFINE_KEYSYM(LMENU)
			DEFINE_KEYSYM(RMENU)
			DEFINE_KEYSYM(OEM_1)
			DEFINE_KEYSYM(OEM_PLUS)
			DEFINE_KEYSYM(OEM_COMMA)
			DEFINE_KEYSYM(OEM_MINUS)
			DEFINE_KEYSYM(OEM_PERIOD)
			DEFINE_KEYSYM(OEM_2)
			DEFINE_KEYSYM(OEM_3)
			DEFINE_KEYSYM(OEM_4)
			DEFINE_KEYSYM(OEM_5)
			DEFINE_KEYSYM(OEM_6)
			DEFINE_KEYSYM(OEM_7)
			DEFINE_KEYSYM(OEM_8)
			DEFINE_KEYSYM(OEM_AX)
			DEFINE_KEYSYM(OEM_102)
			DEFINE_KEYSYM(ICO_HELP)
			DEFINE_KEYSYM(ICO_00)
			DEFINE_KEYSYM(ICO_CLEAR)
			DEFINE_KEYSYM(OEM_RESET)
			DEFINE_KEYSYM(OEM_JUMP)
			DEFINE_KEYSYM(OEM_PA1)
			DEFINE_KEYSYM(OEM_PA2)
			DEFINE_KEYSYM(OEM_PA3)
			DEFINE_KEYSYM(OEM_WSCTRL)
			DEFINE_KEYSYM(OEM_CUSEL)
			DEFINE_KEYSYM(OEM_ATTN)
			DEFINE_KEYSYM(OEM_FINISH)
			DEFINE_KEYSYM(OEM_COPY)
			DEFINE_KEYSYM(OEM_AUTO)
			DEFINE_KEYSYM(OEM_ENLW)
			DEFINE_KEYSYM(OEM_BACKTAB)
			DEFINE_KEYSYM(ATTN)
			DEFINE_KEYSYM(CRSEL)
			DEFINE_KEYSYM(EXSEL)
			DEFINE_KEYSYM(EREOF)
			DEFINE_KEYSYM(PLAY)
			DEFINE_KEYSYM(ZOOM)
			DEFINE_KEYSYM(NONAME)
			DEFINE_KEYSYM(PA1)
			DEFINE_KEYSYM(OEM_CLEAR)

			// special keys (represent keys > 1000 as special)
			m_keyToKeySym[1000] = "LBUTTON"; m_keySymToKey["LBUTTON"] = 1000;
			m_keyToKeySym[1001] = "RBUTTON"; m_keySymToKey["RBUTTON"] = 1001;
			m_keyToKeySym[1002] = "MBUTTON"; m_keySymToKey["MBUTTON"] = 1002;
			m_keyToKeySym[1003] = "MOUSEUP"; m_keySymToKey["MOUSEUP"] = 1003;
			m_keyToKeySym[1004] = "MOUSEDOWN"; m_keySymToKey["MOUSEDOWN"] = 1004;
			m_keyToKeySym[1005] = "MOUSELEFT"; m_keySymToKey["MOUSELEFT"] = 1005;
			m_keyToKeySym[1006] = "MOUSERIGHT"; m_keySymToKey["MOUSERIGHT"] = 1006;
			m_keyToKeySym[1007] = "WHEELUP"; m_keySymToKey["WHEELUP"] = 1007;
			m_keyToKeySym[1008] = "WHEELDOWN"; m_keySymToKey["WHEELDOWN"] = 1008;

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
