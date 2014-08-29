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

#include "GUIDebugPullMenu.hpp"
#include "../Message/MessageDebug.hpp"
#include "../Message/MessageHandler.hpp"

using namespace CEGUI;

CGUIDebugPullMenu::CGUIDebugPullMenu(CEntity *pParentEntity,
                                     CEGUI::Window *pParentWindow,
                                     EPullMenuPositions ePosition)
  : CGUIPullMenu("DebugPullMenu", pParentEntity, pParentWindow, ePosition, 400) {
  
  getDragWindow()->setText("Debug");

  m_pContent = getDragWindow()->createChild("OgreTray/ScrollablePane", "pane");
  m_pContent->setSize(USize::one());

  float fPos = 0;
  createButton("OgreTray/Checkbox", "debug_drawer", "Toggle debug drawer", fPos)->subscribeEvent(ToggleButton::EventSelectStateChanged, Event::Subscriber(&CGUIDebugPullMenu::onToggleDebugDrawer, this));
  createButton("OgreTray/Checkbox", "physics", "Toggle physics debug", fPos)->subscribeEvent(ToggleButton::EventSelectStateChanged, Event::Subscriber(&CGUIDebugPullMenu::onTogglePhysics, this));
}


CEGUI::Window *CGUIDebugPullMenu::createButton(const CEGUI::String &wnd, const CEGUI::String &id, const String &text, float &fPos) {
  Window *pButton = m_pContent->createChild(wnd, id);
  pButton->setSize(USize(UDim(1, 0), UDim(0, 30)));
  pButton->setPosition(UVector2(UDim(0, 0), UDim(0, fPos)));
  pButton->setText(text);
  fPos += 35;

  return pButton;
}

bool CGUIDebugPullMenu::onToggleDebugDrawer(const CEGUI::EventArgs &args) {
  ToggleButton *pTB = dynamic_cast<ToggleButton*>(dynamic_cast<const WindowEventArgs&>(args).window);
  CMessageHandler::getSingleton().addMessage(new CMessageDebug(CMessageDebug::DM_TOGGLE_DEBUG_DRAWER, pTB->isSelected()));
  return true;
}

bool CGUIDebugPullMenu::onTogglePhysics(const CEGUI::EventArgs &args) {
  ToggleButton *pTB = dynamic_cast<ToggleButton*>(dynamic_cast<const WindowEventArgs&>(args).window);
  CMessageHandler::getSingleton().addMessage(new CMessageDebug(CMessageDebug::DM_TOGGLE_PHYSICS, pTB->isSelected()));
  return true;
}