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

#include "../Game.hpp"
#include "GUIDirectionInput.hpp"
#include "../Input/GameInputManager.hpp"

using namespace CEGUI;

CGUIDirectionInput::CGUIDirectionInput(CEntity *pParentEntity,
                                       CEGUI::Window *pParentWindow,
                                       float fPixelSize
                                       )
 : CGUITouchButton("input_direction", pParentEntity, pParentWindow, pParentWindow->createChild("DefaultWindow", "input_direction_root")),
   m_vDotCenter(fPixelSize / 2, pParentWindow->getPixelSize().d_height - fPixelSize / 2),
   m_fMaxRadius(0.25 * fPixelSize) {

  m_pRoot->setSize(USize(UDim(0, fPixelSize), UDim(0, fPixelSize)));
  m_pRoot->setPosition(UVector2(UDim(0, 0), UDim(1, -fPixelSize)));

  m_pDot = m_pRoot->createChild("OgreTray/Label", "Dot");
  m_pDot->setPosition(UVector2(UDim(0.45, 0), UDim(0.45, 0)));
  m_pDot->setSize(USize(UDim(0.1, 0), UDim(0.1, 0)));
  m_pDot->setText("O");
}

CGUIDirectionInput::~CGUIDirectionInput() {
}

void CGUIDirectionInput::handleButtonPress(bool bActive) {

  if (!bActive) {
    // update input
    OIS::Keyboard *pKeyboard = CGame::getSingleton().getInputContext().mKeyboard;
    if (pKeyboard && !m_bHitOnce) {
      if (pKeyboard->isKeyDown(OIS::KC_LEFT)) {
        m_vCurrentDotPosition.d_x = -m_fMaxRadius;
        m_bHitOnce = true;
      }
      else if (pKeyboard->isKeyDown(OIS::KC_RIGHT)) {
        m_vCurrentDotPosition.d_x = +m_fMaxRadius;
        m_bHitOnce = true;
      }
      else {
        m_vCurrentDotPosition.d_x = 0;
      }
      if (pKeyboard->isKeyDown(OIS::KC_UP)) {
        m_vCurrentDotPosition.d_y = -m_fMaxRadius;
        m_bHitOnce = true;
      }
      else if (pKeyboard->isKeyDown(OIS::KC_DOWN)) {
        m_vCurrentDotPosition.d_y = m_fMaxRadius;
        m_bHitOnce = true;
      }
      else {
        m_vCurrentDotPosition.d_y = 0;
      }
    }
  }

  if (!m_bHitOnce) {
    m_vCurrentDotPosition = Vector2f::zero();
  }

  m_pDot->setPosition(UVector2(UDim(0.45, m_vCurrentDotPosition.d_x), UDim(0.45, m_vCurrentDotPosition.d_y)));

  if (m_vCurrentDotPosition.d_x < 0) {
    CGameInputManager::getSingleton().
      injectCommand(CGameInputCommand(GIC_LEFT,	-m_vCurrentDotPosition.d_x));
  }
  else {
    CGameInputManager::getSingleton().
      injectCommand(CGameInputCommand(GIC_RIGHT, m_vCurrentDotPosition.d_x));
  }

  if (m_vCurrentDotPosition.d_y < 0) {
    CGameInputManager::getSingleton().
      injectCommand(CGameInputCommand(GIC_FRONT, -m_vCurrentDotPosition.d_y));
  }
  else {
    CGameInputManager::getSingleton().
      injectCommand(CGameInputCommand(GIC_REAR, m_vCurrentDotPosition.d_y));
  }
}

void CGUIDirectionInput::processInput(const CEGUI::Vector2f &vPosition) {
  if (m_pRoot->isHit(vPosition)) {
    Vector2f vDir = vPosition - m_vDotCenter;
    float fLength = sqrt(vDir.d_x * vDir.d_x + vDir.d_y * vDir.d_y);
    vDir = vDir / fLength;
    m_vCurrentDotPosition = vDir * std::min<float>(fLength, m_fMaxRadius);
    m_bHitOnce = true;
  }
}