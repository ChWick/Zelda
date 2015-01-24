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
#include "../../Common/Message/MessageEntityDataChanged.hpp"
#include "../../Common/Message/MessageHitpointsChanged.hpp"
#include "../Messages/MessageItem.hpp"
#include "../Messages/UserMessageTypes.hpp"
#include "../../Common/World/HitableInterface.hpp"
#include "../WorldEntity.hpp"
#include "../Items/ItemData.hpp"
#include "../UserImplementations/EntityPropertyIds.hpp"
#include "../../Common/GameLogic/EntityProperty.hpp"

using namespace CEGUI;

CHUD::CHUD(CEntity *pParentEntity, CEGUI::Window *pParentWindow)
  : CGUIOverlay("hud", pParentEntity, pParentWindow, pParentWindow->createChild("DefaultWindow", "hud_root")) {

  Window *m_pLivesText = m_pRoot->createChild("OgreTray/ShadowedLabel", "lives_text");
  m_pLivesText->setText("-- LIFE --");
  m_pLivesText->setPosition(UVector2(UDim(0.5, 0), UDim(0, 0)));
  m_pLivesText->setSize(USize(UDim(0.5, 0), UDim(0.05, 0)));
  m_pLivesText->setProperty("NormalTextColour", "FFFFFFFF");

  m_pCurrentItemDisplay = m_pRoot->createChild("OgreTray/StaticImage", "item_display");
  m_pCurrentItemDisplay->setPosition(UVector2(UDim(0.2, 0), UDim(0.05, 0)));
  m_pCurrentItemDisplay->setSize(USize(UDim(0, 60), UDim(0, 60)));
  m_pCurrentItemDisplay->setProperty("Image", "hud/Bow");

  m_pHeartsDisplay = new CGUIHeartsDisplay(this, m_pRoot, UVector2(UDim(0.5, 0), UDim(0.05, 0)));

  m_pRupeeCounter = new CGUICounter("counter_rupee", this, m_pRoot, 4, "hud/Rupee", UVector2(UDim(0.3, 0), UDim(0.05, 0)));
  m_pBombCounter = new CGUICounter("counter_bomb", this, m_pRoot, 4, "hud/Bomb", UVector2(UDim(0.35, 0), UDim(0.05, 0)));
  m_pArrowCounter = new CGUICounter("counter_arrow", this, m_pRoot, 4, "hud/Arrows", UVector2(UDim(0.4, 0), UDim(0.05, 0)));
}

void CHUD::handleMessage(const CMessagePtr message) {
  if (message->getType() == MSG_HITPOINTS_CHANGED) {
    auto msg_hp_change(
        std::dynamic_pointer_cast<const CMessageHitpointsChanged>(message));
    if (dynamic_cast<const CWorldEntity *>(&msg_hp_change->getHitableInterface()) && dynamic_cast<const CWorldEntity&>(msg_hp_change->getHitableInterface()).getID() == "player") {
      m_pHeartsDisplay->changeMaximalHitpoints(
          msg_hp_change->getHitableInterface().getMaxHP());
      m_pHeartsDisplay->changeCurrentHitpoints(
          msg_hp_change->getHitableInterface().getCurrentHP());
    }
  }
  else if (message->getType() == MSG_ITEM) {
    auto msg_item(std::dynamic_pointer_cast<const CMessageItem>(message));
    if (msg_item->getItemMessageType() == CMessageItem::IM_SELECTION_CHANGED) {
      m_pCurrentItemDisplay->setProperty(
          "Image",
          "hud/" + CItemVariantDataMap::getSingleton().toData(
              msg_item->getItemVariantType()).sImagesetName);
    }
  }
  else if (message->getType() == MSG_ENTITY_DATA_CHANGED) {
    auto msg_edc(std::dynamic_pointer_cast<const CMessageEntityDataChanged>(message));
    int propertyId(msg_edc->getProperty().getID());
    if (propertyId == ENTITY_PROPERTY_RUPEE) {
      m_pRupeeCounter->setDesiredCount(
          dynamic_cast<const CEntityIntProperty&>(msg_edc->getProperty()).getData());
    }
  }
}
