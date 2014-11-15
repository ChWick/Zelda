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

#include "WorldGUIItemViewer.hpp"
#include "WorldGUIItemSelector.hpp"
#include "../../Common/Input/GameInputCommand.hpp"

using namespace CEGUI;

CWorldGUIItemViewer::CWorldGUIItemViewer(CEntity *pParentEntity, CEGUI::Window *pParentWindow)
  : CGUIPullMenu("world_gui_item_viewer", pParentEntity, pParentWindow, PMD_TOP, -1, "DefaultWindow") {

  CWorldGUIItemSelector *pItemSelector = new CWorldGUIItemSelector(this, getDragWindow());
  pItemSelector->getRoot()->setSize(USize(UDim(0.667, -5), UDim(0.667, -5)));
  m_pWorldGUIItemSelector = pItemSelector;

  Window *pDoWindow = getDragWindow()->createChild("OgreTray/Group", "do");
  pDoWindow->setText("DO");
  pDoWindow->setPosition(UVector2(UDim(0, 0), UDim(0.667, 5)));
  pDoWindow->setSize(USize(UDim(0.667, -5), UDim(0.333, -5)));

  Window *pEquipmentWindow = getDragWindow()->createChild("OgreTray/Group", "equipment");
  pEquipmentWindow->setText("EQUIPMENT");
  pEquipmentWindow->setPosition(UVector2(UDim(0.667, 5), UDim(0.667, 5)));
  pEquipmentWindow->setSize(USize(UDim(0.333, -5), UDim(0.333, -5)));
}

void CWorldGUIItemViewer::receiveInputCommand(const CGameInputCommand &cmd) {
  if (cmd.getType() == GIC_SHOW_MENU && cmd.getState() == GIS_PRESSED) {
    toggle();
  }
}

void CWorldGUIItemViewer::onPullStarted() {
  pause(PAUSE_ATLAS_UPDATE);
  m_pWorldGUIItemSelector->stop();
}

void CWorldGUIItemViewer::onClosed() {
  unpause(PAUSE_ATLAS_UPDATE);
}

void CWorldGUIItemViewer::onOpened() {
  m_pWorldGUIItemSelector->start();
}
