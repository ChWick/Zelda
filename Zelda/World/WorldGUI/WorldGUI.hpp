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

#ifndef _WORLD_GUI_HPP_
#define _WORLD_GUI_HPP_

#include "../../Common/GUI/GUIOverlay.hpp"

class CHUD;

class CWorldGUI : public CGUIOverlay {
private:
  CHUD *m_pHUD;
public:
  CWorldGUI(CEntity *pParentEntity);
  ~CWorldGUI();

protected:
  void handleMessage(const CMessage &message);
};
#endif // _WORLD_GUI_HPP_
