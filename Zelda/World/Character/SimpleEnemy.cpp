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

#include "SimpleEnemy.hpp"
#include "SimpleEnemyController.hpp"
#include "../Atlas/Map.hpp"
#include "../Damage.hpp"
#include "PersonTypes.hpp"

CSimpleEnemy::CSimpleEnemy(const std::string &sID, CEntity *pParent, EEnemyTypes eEnemyType)
	: CPerson(sID, pParent, PERSON_TYPE_ID_MAP.toData(PERSON_SOLDIER_BLUE)), m_eEnemyType(eEnemyType) {
}
void CSimpleEnemy::setupInternal() {
	/*switch (m_eEnemyType) {
	case ET_GREEN_SWORD:
		createTool(CPlayerTool::TOOL_SWORD, true);
		createShield(CShield::ST_SIMPLE_SHIELD, true);
		setCurAndMaxHP(HP_ONE_HEART);
		m_bSwordsDrawn = true;
		break;
    case ET_BLOCKER:
		createTool(CPlayerTool::TOOL_SWORD, true);
		createShield(CShield::ST_SIMPLE_SHIELD, true);
		m_bSwordsDrawn = true;
		animStartBlock();
		setCurAndMaxHP(HP_INFINITY);
		dynamic_cast<CSimpleEnemyController*>(m_pCharacterController)->stun();
		m_uiTakeDamageFlags = CDamage::DMG_NONE;
		break;
	}*/
}
CCharacterController *CSimpleEnemy::createCharacterController() {
	return new CSimpleEnemyController(this);
}
void CSimpleEnemy::killedCallback() {
	//m_pCurrentMap->destroyLater(this);
	//InnerObjectGenerator::createInnerObject(*m_pCurrentMap, getPosition(), InnerObjectGenerator::IOLT_DEFAULT);
}
void CSimpleEnemy::setPlayer(CPlayer *pPlayer) {
	dynamic_cast<CSimpleEnemyController*>(m_pCharacterController)->setPlayer(pPlayer);
}
