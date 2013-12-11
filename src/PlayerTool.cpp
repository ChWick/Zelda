#include "StdAfx.h"
#include "PlayerTool.h"
#include "HUD.h"

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
CDamage::EType CPlayerTool::DMG_TYPE[] = {
    CDamage::DMG_NONE,
    CDamage::DMG_SWORD,
    CDamage::DMG_ARROW,
    CDamage::DMG_HAMMER,
    CDamage::DMG_BOMB,
    CDamage::DMG_FIRE,
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
    CHUD::getSingleton().setCurrentTool(getCurrentTool());
}
