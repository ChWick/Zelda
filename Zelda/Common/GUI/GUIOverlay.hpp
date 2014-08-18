/*****************************************************************************
 * Copyright 2014 Christoph Wick
 *
 * This file is part of Mencus.
 *
 * Mencus is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Mencus is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Mencus. If not, see http://www.gnu.org/licenses/.
 *****************************************************************************/

#ifndef _GUI_OVERLAY_HPP_
#define _GUI_OVERLAY_HPP_

#include <CEGUI/CEGUI.h>
#include "../GameLogic/Entity.hpp"

class CGUIOverlay : public CEntity {
protected:
  CEGUI::Window *m_pParentWindow;
public:
  CGUIOverlay(const std::string &id, CEntity *pParentEntity, CEGUI::Window *pParentWindow)
  : CEntity(id, pParentEntity),
    m_pParentWindow(pParentWindow) {
  }
  virtual ~CGUIOverlay() {}

  virtual void changeTouchButtonSize(float fSize) {}
  virtual void onGUIScalingChanged(float fScaling) {}
private:
};

#endif
