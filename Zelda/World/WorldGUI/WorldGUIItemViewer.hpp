/*****************************************************************************
 * Copyright 2014 Christoph Wick
 *
 * This file is part of Zelda.
 *
 * Zelda is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Zelda is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Zelda. If not, see http://www.gnu.org/licenses/.
 *****************************************************************************/

#ifndef _WORLD_GUI_ITEM_VIEWER_HPP_
#define _WORLD_GUI_ITEM_VIEWER_HPP_

#include "../../Common/GUI/GUIOverlay.hpp"
#include "../../Common/GUI/GUIPullMenu.hpp"
#include "../../Common/PauseManager/PauseCaller.hpp"
#include "../../Common/Input/GameInputListener.hpp"

class CWorldGUIItemSelector;

class CWorldGUIItemViewer : public CGUIPullMenu,
			    public CPauseCaller,
			    public CGameInputListener {
private:
  CWorldGUIItemSelector *m_pWorldGUIItemSelector;
public:
  CWorldGUIItemViewer(CEntity *pParentEntity, CEGUI::Window *pParentWindow);

  void receiveInputCommand(const CGameInputCommand &cmd);
private:
  void onPullStarted();
  void onClosed();
  void onOpened();
};

#endif // _WORLD_GUI_ITEM_VIEWER_HPP_
