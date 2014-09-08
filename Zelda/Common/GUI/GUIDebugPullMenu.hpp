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

#ifndef __Zelda__GUIDebugPullMenu__
#define __Zelda__GUIDebugPullMenu__

#include "GUIPullMenu.hpp"

class CGUIDebugPullMenu : public CGUIPullMenu {
private:
  CEGUI::Window *m_pContent;
  CEGUI::Window *m_pFrameStatsGroup;
  CEGUI::Window *m_pAverageFps;
  CEGUI::Window *m_pBestFps;
  CEGUI::Window *m_pWorstFps;
  CEGUI::Window *m_pBatches;
  CEGUI::Window *m_pTriangles;
public:
  CGUIDebugPullMenu(CEntity *pParentEntity,
                    CEGUI::Window *pParentWindow,
                    EPullMenuPositions ePosition);

private:
  CEGUI::Window *createButton(const CEGUI::String &wnd, const CEGUI::String &id, const CEGUI::String &text, float &fPos);
  CEGUI::Window *createFrameStatsButton(const CEGUI::String &id, const CEGUI::String &sLabel, CEGUI::Window *&pWindow, int iIndex);

  void update(Ogre::Real tpf);

  bool onToggleDebugDrawer(const CEGUI::EventArgs &args);
  bool onTogglePhysics(const CEGUI::EventArgs &args);
};

#endif /* defined(__Zelda__GUIDebugPullMenu__) */
