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

#include "GUIHeartsDisplay.hpp"

using namespace CEGUI;

const float GUI_HEART_IMG_SIZE = 32;
const float GUI_HEART_IMG_WITH_BORDER_SIZE = 32;

CGUIHeartsDisplay::CGUIHeartsDisplay(CEntity *pParentEntity, CEGUI::Window *pParentWindow, const CEGUI::UVector2 &vPosition)
  : CGUIOverlay("gui_hearts_display", pParentEntity, pParentWindow, pParentWindow->createChild("DefaultWindow")),
    m_MaximalHitpoints(0),
    m_CurrentHitpoints(0) {

  m_pRoot->setPosition(vPosition);
  m_pRoot->setSize(USize(UDim(0, 10 * GUI_HEART_IMG_WITH_BORDER_SIZE), UDim(0, 2 * GUI_HEART_IMG_WITH_BORDER_SIZE)));

  for (Ogre::uint16 i = 0; i < m_uiMaxHeartCount; i++) {
		CEGUI::Window *pWindow = m_pRoot->createChild("OgreTray/StaticImage", "heart_" + CEGUI::PropertyHelper<int>::toString(i) );
		pWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0, (i % (m_uiMaxHeartCount / 2)) * GUI_HEART_IMG_WITH_BORDER_SIZE),CEGUI::UDim(0, (i / (m_uiMaxHeartCount / 2))* GUI_HEART_IMG_WITH_BORDER_SIZE)));
		pWindow->setSize(CEGUI::USize(CEGUI::UDim(0, GUI_HEART_IMG_SIZE),CEGUI::UDim(0, GUI_HEART_IMG_SIZE)));
		pWindow->setProperty("Image","hud/0/4");
		pWindow->setProperty("FrameEnabled","False");
		pWindow->setProperty("BackgroundEnabled","False");
		pWindow->setVisible(false);
	}
}

void CGUIHeartsDisplay::changeCurrentHitpoints(Hitpoints hp) {
  m_CurrentHitpoints = hp;
  updateHitpoints();
}
void CGUIHeartsDisplay::changeMaximalHitpoints(Hitpoints hp) {
  m_MaximalHitpoints = hp;
  updateHitpoints();
}
void CGUIHeartsDisplay::updateHitpoints() {
  int fullHearts = m_CurrentHitpoints / HP_ONE_HEART;
	int quarterHearts = (m_CurrentHitpoints - fullHearts * HP_ONE_HEART) / HP_QUARTER_HEART;

	for (unsigned int i = 0; i < m_uiMaxHeartCount; i++) {
		Hitpoints hp = i * HP_ONE_HEART;

		Window *pHeart = m_pRoot->getChild("heart_" + CEGUI::PropertyHelper<int>::toString(i));

		if (hp >= m_MaximalHitpoints) {
			pHeart->setVisible(false);
		}
		else if (hp < fullHearts * HP_ONE_HEART) {
			pHeart->setVisible(true);
			pHeart->setProperty("Image","hud/4/4");
		}
		else if (hp < (fullHearts + 1)* HP_ONE_HEART) {
			pHeart->setVisible(true);
			pHeart->setProperty("Image","hud/" + CEGUI::PropertyHelper<int>::toString(quarterHearts) + "/4");
		}
		else {
			pHeart->setVisible(true);
			pHeart->setProperty("Image","hud/0/4");
		}
	}

}
