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

#include "../../Common/GUI/GUIManager.hpp"
#include "WorldGUI.hpp"
#include "../../Common/GUI/GUIPullMenu.hpp"
#include "HUD.hpp"
#include "../../Common/GUI/GUIDirectionInput.hpp"
#include "WorldGUISimpleTouchButtons.hpp"
#include "WorldGUIItemViewer.hpp"
#include "../../GUIComponents/GUITextBox.hpp"
#include "../Messages/UserMessageTypes.hpp"
#include "../Messages/MessageShowText.hpp"
#include "GlobalBuildDefines.hpp"
#include "../../Common/Log.hpp"


CWorldGUI::CWorldGUI(CEntity *pParentEntity)
  : CGUIOverlay("world_gui", pParentEntity, CGUIManager::getSingleton().getRoot(),
                CGUIManager::getSingleton().getRoot()->createChild("DefaultWindow", "world_gui_root")) {

  CGUIManager::getSingleton().addGUIOverlay(this);

  m_pRoot->setMousePassThroughEnabled(true);

  new CWorldGUIItemViewer(this, m_pRoot);

  m_pHUD = new CHUD(this, m_pRoot);

#if ENABLE_INPUT_TOUCH == 1
  new CGUIDirectionInput(this, m_pRoot, m_pRoot->getPixelSize().d_width * 0.2);
  new CWorldGUISimpleTouchButton("interact", this, m_pRoot, STB_INTERACT, m_pRoot->getPixelSize().d_width * 0.14);
  new CWorldGUISimpleTouchButton("run", this, m_pRoot, STB_RUN, m_pRoot->getPixelSize().d_width * 0.14);
  new CWorldGUISimpleTouchButton("sword", this, m_pRoot, STB_SWORD, m_pRoot->getPixelSize().d_width * 0.14);
#endif
}

CWorldGUI::~CWorldGUI() {
  CGUIManager::getSingleton().removeGUIOverlay(this);
}

void CWorldGUI::handleMessage(const CMessagePtr message) {
  if (message->getType() == MSG_SHOW_TEXT) {
    auto msg_show_text(std::dynamic_pointer_cast<const CMessageShowText>(message));
    if (msg_show_text->getStatus() == CMessageShowText::REQUEST) {
      LOGV("Creating MessageBox in CWorldGUI");
      new CGUITextBox("text_box", this, m_pRoot, msg_show_text->getLanguageString(), msg_show_text->getResult());
    }
  }
}
