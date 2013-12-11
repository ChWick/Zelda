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

#include "PlayerTool.hpp"

Ogre::String CPlayerTool::MESH_FILES[] = {
    "BLANK_MESH_FILE",
    "SimpleSword.mesh",
    "Bow.mesh",
    "Hammer.mesh",
    "Bomb.mesh",
    "LampHandle.mesh",
};

CEGUI::String CPlayerTool::HUD_IMAGES[] = {
    "hud/Blank",
    "hud/Sword",
    "hud/Bow",
    "hud/Hammer",
    "hud/Bomb",
    "hud/Lamp",
};
EDamageType CPlayerTool::DMG_TYPE[] = {
    DMG_NONE,
    DMG_SWORD,
    DMG_ARROW,
    DMG_HAMMER,
    DMG_BOMB,
    DMG_FIRE,
};


CPlayerTool::CPlayerTool(EPlayerToolType eToolType)
: m_eToolType(eToolType), m_bExists(true) {
}


CPlayerTools::CPlayerTools()
:
    /*m_aTools(
        {
            CPlayerTool(CPlayerTool::TOOL_BLANK),
            CPlayerTool(CPlayerTool::TOOL_SWORD),
            CPlayerTool(CPlayerTool::TOOL_BOW),
            CPlayerTool(CPlayerTool::TOOL_HAMMER),
            CPlayerTool(CPlayerTool::TOOL_BOMB),
            CPlayerTool(CPlayerTool::TOOL_LAMP)
        }),*/
    m_uiCurrentTool(CPlayerTool::TOOL_BLANK)

    {
		for (int i = 0; i < CPlayerTool::TOOL_COUNT; i++) {
			m_aTools[i] = CPlayerTool(static_cast<CPlayerTool::EPlayerToolType>(i));
		}

    // 'blank' is the only tool that always exists
    m_aTools[CPlayerTool::TOOL_BLANK].setExists(true);
}
void CPlayerTools::selectNextTool() {
    m_uiCurrentTool++;
    if (m_uiCurrentTool >= CPlayerTool::TOOL_COUNT) {
        m_uiCurrentTool = 0;
    }

    if (getCurrentTool().doesExists() == false) {
        selectNextTool();
        return;
    }

    updateCurrentTool();
}
void CPlayerTools::selectPreviousTool() {
    if (m_uiCurrentTool == 0) {
        m_uiCurrentTool = CPlayerTool::TOOL_COUNT - 1;
    }
    else {
        m_uiCurrentTool--;
    }

    if (getCurrentTool().doesExists() == false) {
        selectPreviousTool();
        return;
    }

    updateCurrentTool();
}
void CPlayerTools::updateCurrentTool() {
}
