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

#ifndef _PLAYER_TOOL_H_
#define _PLAYER_TOOL_H_

#include "../../Common/World/Damage.hpp"
#include <CEGUI/String.h>

//!< This class represents the tools the player can use
/**
  * e.g. a sword, the bow, the lamp, and all the other stuff
  */
class CPlayerTool {
public:
    static Ogre::String MESH_FILES[];
    static CEGUI::String HUD_IMAGES[];
    static EDamageType DMG_TYPE[];

    enum EPlayerToolType {
        TOOL_BLANK,                         //!< no tool selected
        TOOL_SWORD,                         //!< sword
        TOOL_BOW,                           //!< bow
        TOOL_HAMMER,                        //!< hammer
        TOOL_BOMB,                          //!< bomb
        TOOL_LAMP,                          //!< lamp

        TOOL_COUNT,
    };

private:
    EPlayerToolType m_eToolType;      //!< Type of the tool
    bool m_bExists;                         //!< Has the player already this tool?
public:
    //! constructor
    /**
      * @param eToolType The type of the tool
      * @param sImage The id of the image in the hud imageset
      */
    CPlayerTool(EPlayerToolType eToolType = TOOL_COUNT);

	void operator=(const CPlayerTool &src) {
		m_eToolType = src.m_eToolType;
		m_bExists = src.m_bExists;
	}


    void setExists(bool bExists) {m_bExists = bExists;}
    bool doesExists() const {return m_bExists;}
    EPlayerToolType getToolType() const {return m_eToolType;}
    const CEGUI::String &getImage() const {return HUD_IMAGES[m_eToolType];}
    const Ogre::String &getMesh() const {return MESH_FILES[m_eToolType];}
};

//! This class holds all the possible tools
class CPlayerTools {
private:
    CPlayerTool m_aTools[CPlayerTool::TOOL_COUNT];      //!< Array to store all the tools

    unsigned int m_uiCurrentTool;                       //!< Stores the current tool of the player
public:
    CPlayerTools();

    void selectNextTool();
    void selectPreviousTool();

    const CPlayerTool &getCurrentTool() const {return m_aTools[m_uiCurrentTool];}
    const CPlayerTool &getTool(unsigned int uiToolType) const {return m_aTools[uiToolType];}
    CPlayerTool &getTool(unsigned int uiToolType) {return m_aTools[uiToolType];}
    unsigned int getNumTools() const {return CPlayerTool::TOOL_COUNT;}

private:
    void updateCurrentTool();
};

#endif
