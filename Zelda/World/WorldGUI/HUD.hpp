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

#ifndef _HUD_HPP_
#define _HUD_HPP_

#include "../../Common/GUI/GUIOverlay.hpp"

class CGUIHeartsDisplay;
class CGUICounter;

class CHUD : public CGUIOverlay {
private:
  CGUIHeartsDisplay *m_pHeartsDisplay;
  CGUICounter *m_pRupeeCounter;
  CGUICounter *m_pBombCounter;
  CGUICounter *m_pArrowCounter;
  CEGUI::Window *m_pCurrentItemDisplay;
public:
  CHUD(CEntity *pParentEntity, CEGUI::Window *pParentWindow);

private:
  void handleMessage(const CMessage &message);
};
#endif // _HUD_HPP_
