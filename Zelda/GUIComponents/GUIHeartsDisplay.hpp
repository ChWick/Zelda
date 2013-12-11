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

#ifndef _GUI_HEARTS_DISPLAY_HPP_
#define _GUI_HEARTS_DISPLAY_HPP_

#include "../Common/GUI/GUIOverlay.hpp"
#include "../World/Hitpoints.hpp"


class CGUIHeartsDisplay : public CGUIOverlay {
private:
  static const Ogre::uint16 m_uiMaxHeartCount = 20;

  Hitpoints m_MaximalHitpoints;
  Hitpoints m_CurrentHitpoints;
public:
  CGUIHeartsDisplay(CEntity *pParentEntity, CEGUI::Window *pParentWindow, const CEGUI::UVector2 &vPosition = CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));


  void changeCurrentHitpoints(Hitpoints hp);
  void changeMaximalHitpoints(Hitpoints hp);
private:

  void updateHitpoints();
};

#endif // _GUI_HEARTS_DISPLAY_HPP_
