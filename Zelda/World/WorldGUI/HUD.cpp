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

#include "HUD.hpp"
#include "../../GUIComponents/GUIHeartsDisplay.hpp"
#include "../../GUIComponents/GUICounter.hpp"
#include "../../Common/Message/MessagePlayerPickupItem.hpp"
#include "../../Common/Message/MessageHitpointsChanged.hpp"
#include "../HitableInterface.hpp"
#include "../WorldEntity.hpp"

using namespace CEGUI;

CHUD::CHUD(CEntity *pParentEntity, CEGUI::Window *pParentWindow)
  : CGUIOverlay("hud", pParentEntity, pParentWindow, pParentWindow->createChild("DefaultWindow", "hud_root")) {

  Window *m_pLivesText = m_pRoot->createChild("OgreTray/ShadowedLabel", "lives_text");
  m_pLivesText->setText("-- LIFE --");
  m_pLivesText->setPosition(UVector2(UDim(0.5, 0), UDim(0, 0)));
  m_pLivesText->setSize(USize(UDim(0.5, 0), UDim(0.05, 0)));
  m_pLivesText->setProperty("NormalTextColour", "FFFFFFFF");

  m_pHeartsDisplay = new CGUIHeartsDisplay(this, m_pRoot, UVector2(UDim(0.5, 0), UDim(0.05, 0)));

  m_pRupeeCounter = new CGUICounter("counter_rupee", this, m_pRoot, 4, "hud/Rupee", UVector2(UDim(0.3, 0), UDim(0.05, 0)));
  m_pBombCounter = new CGUICounter("counter_bomb", this, m_pRoot, 4, "hud/Bomb", UVector2(UDim(0.35, 0), UDim(0.05, 0)));
  m_pArrowCounter = new CGUICounter("counter_arrow", this, m_pRoot, 4, "hud/Arrows", UVector2(UDim(0.4, 0), UDim(0.05, 0)));
}

void CHUD::handleMessage(const CMessage &message) {
  if (message.getType() == MSG_PLAYER_PICKUP_ITEM) {
    m_pRupeeCounter->addCount(10);
  }
  else if (message.getType() == MSG_HITPOINTS_CHANGED) {
    const CMessageHitpointsChanged &msg_hp_change(dynamic_cast<const CMessageHitpointsChanged&>(message));
    if (dynamic_cast<const CWorldEntity *>(&msg_hp_change.getHitableInterface()) && dynamic_cast<const CWorldEntity&>(msg_hp_change.getHitableInterface()).getID() == "player") {
      m_pHeartsDisplay->changeMaximalHitpoints(msg_hp_change.getHitableInterface().getMaxHP());
      m_pHeartsDisplay->changeCurrentHitpoints(msg_hp_change.getHitableInterface().getCurrentHP());
    }
  }
}