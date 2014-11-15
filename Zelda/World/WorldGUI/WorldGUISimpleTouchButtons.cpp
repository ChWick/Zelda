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

#include "WorldGUISimpleTouchButtons.hpp"
#include "../../Common/Input/GameInputManager.hpp"

using namespace CEGUI;

CWorldGUISimpleTouchButton::CWorldGUISimpleTouchButton(const std::string &id,
                                                       CEntity *pParentEntity,
                                                       CEGUI::Window *pParentWindow,
                                                       ESimpleTouchButtons eType,
                                                       float fButtonPixelSize)
  : CGUITouchButton(id, pParentEntity, pParentWindow, pParentWindow->createChild("OgreTray/StaticImage", id + "_root")) {

  setType(eType);

  m_pRoot->setSize(USize(UDim(0, fButtonPixelSize), UDim(0, fButtonPixelSize)));

  switch (m_uiType) {
  case STB_INTERACT:
    m_pRoot->setPosition(UVector2(UDim(1, -fButtonPixelSize), UDim(0, 0)));
    break;
  case STB_RUN:
    m_pRoot->setPosition(UVector2(UDim(1, -fButtonPixelSize), UDim(0, fButtonPixelSize)));
    break;
  case STB_SWORD:
    m_pRoot->setPosition(UVector2(UDim(1, -fButtonPixelSize), UDim(0, 2 * fButtonPixelSize)));
    break;
  default:
    break;
  }
}

void CWorldGUISimpleTouchButton::handleButtonPress(bool bActive) {
  switch (m_uiType) {
  case STB_INTERACT:
    CGameInputManager::getSingleton().injectCommand(CGameInputCommand(GIC_INTERACT, bActive));
    break;
  case STB_RUN:
    CGameInputManager::getSingleton().injectCommand(CGameInputCommand(GIC_RUN, bActive));
    break;
  case STB_SWORD:
    CGameInputManager::getSingleton().injectCommand(CGameInputCommand(GIC_SWORD, bActive));
    break;

  default:
    break;
  }
}