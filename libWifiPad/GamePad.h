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
#ifndef __GamePad_h
#define __GamePad_h

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "Mutex.h"
#include "Uuid.h"
#include "Image.h"

namespace WifiPad
{
	// holds coordinate/key information about the button
	struct ButtonInfo
	{
		// the rect to trigger the button press
		int x;
		int y;
		int width;
		int height;

		ImagePtr image; // pointer to the button down image
		int imageX; // x coordinate for the button down image
		int imageY; // x coordinate for the button down image

		bool isTrackPad; // this button acts as a trackpad
		
		int defaultKey[3];

		ButtonInfo() : x(0), y(0), width(0), height(0), imageX(0), imageY(0), isTrackPad(false) {
			defaultKey[0] = defaultKey[1] = defaultKey[2] = -1;
		}
	};

	class DataSource;

	/*
	 * This class represents a GamePad.
	 */
	class GamePad
	{
	private:
		std::string m_name; // Name of the game pad
		Uuid m_uuid; // UUID of the game pad
		ImagePtr m_gamepadImage; // image of the gamepad
		std::vector<ButtonInfo> m_buttons;
		mutable Mutex m_lock, m_loadLock; // lock
	public:
		GamePad(const std::string& name = "No name") : m_name(name) { }
		GamePad(DataSource& source,int loadLevel = 1) { LoadGamePad(source,loadLevel); }

		// deletes gamepad data
		void Clear();

		// load gamepad from source, where loadLevel is how much data to load
		// loadLevel = 0, load header and button information only, no images are loaded
		// loadLevel = 1, load everything, including images
		void LoadGamePad(DataSource& source,int loadLevel = 1);
		void SaveGamePad(const std::string& filename) const;

		std::string GetName() const { ScopedLock lock(m_lock); return m_name; }
		Uuid GetUuid() const { ScopedLock lock(m_lock); return m_uuid; }
		ImagePtr GetMainImage() { ScopedLock lock(m_lock); return m_gamepadImage; }

		ButtonInfo& GetButton(int n) { ScopedLock lock(m_lock); return m_buttons[n]; }
		const ButtonInfo& GetButton(int n) const { ScopedLock lock(m_lock); return m_buttons[n]; }
		ButtonInfo& CreateButton() {
			ScopedLock lock(m_lock); 
			m_buttons.resize(m_buttons.size() + 1);
			return m_buttons[m_buttons.size() - 1];
		}
		void AddButton(const ButtonInfo& button) { ScopedLock lock(m_lock); m_buttons.push_back(button); }

		int GetNumButtons() const { ScopedLock lock(m_lock); return (int)m_buttons.size(); }
	};
	
	typedef boost::shared_ptr<GamePad> GamePadPtr;
}

#endif
