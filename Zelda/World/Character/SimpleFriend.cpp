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

#include "SimpleFriend.hpp"
#include "PlayerTool.hpp"
#include "PersonTypes.hpp"

SimpleFriend::SimpleFriend(const std::string &sID, CEntity *pParent, ESimpleFriendTypes eType)
: CPerson(sID, pParent, PERSON_TYPE_ID_MAP.toData(PERSON_SOLDIER_BLUE)), m_eType(eType){
    //ctor
}

SimpleFriend::~SimpleFriend()
{
    //dtor
}
void SimpleFriend::setupInternal() {
	switch (m_eType) {
	case SF_LINKS_FATHER:
		//createTool(CPlayerTool::TOOL_SWORD, true);
		//createShield(CShield::ST_SIMPLE_SHIELD, true);
		//m_bSwordsDrawn = true;
		break;
	}
}

void SimpleFriend::setupAnimations() {
}