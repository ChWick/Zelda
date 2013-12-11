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

#ifndef _GUI_COUNTER_HPP_
#define _GUI_COUNTER_HPP_

#include "../Common/GUI/GUIOverlay.hpp"

class CGUICounter : public CGUIOverlay {
private:
  int m_iCurrentCount;
  int m_iDesiredCount;
  Ogre::Real m_fTimeSinceLastAdded;
  const Ogre::Real m_fCountsPerSecond;

  CEGUI::Window *m_pCounter;
public:
  CGUICounter(const std::string &id,
              CEntity *pParentEntity,
              CEGUI::Window *pParentWindow,
              Ogre::Real fCountsPerSecond,
              const CEGUI::String &sImage,
              const CEGUI::UVector2 &vPosition = CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));

  void setCurrentCount(int iNewCount, bool bAnimate = true);

  void update(Ogre::Real tpf);

private:
  void updateText();
};

#endif // _GUI_COUNTER_HPP_
