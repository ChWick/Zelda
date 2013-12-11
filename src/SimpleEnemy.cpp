#include "StdAfx.h"
#include "SimpleEnemy.h"
#include "SimpleEnemyController.h"
#include "Map.h"
#include "Damage.h"
#include "PickableObject.h"

CSimpleEnemy::CSimpleEnemy(EEnemyTypes eEnemyType, CMap *pMap)
	: CPerson(pMap, FOE_ENEMY), m_eEnemyType(eEnemyType) {
}
void CSimpleEnemy::setupInternal() {
	switch (m_eEnemyType) {
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
	}
}
CCharacterController *CSimpleEnemy::createCharacterController() {
	return new CSimpleEnemyController(this);
}
void CSimpleEnemy::killedCallback() {
	m_pCurrentMap->destroyLater(this);
	InnerObjectGenerator::createInnerObject(*m_pCurrentMap, getPosition(), InnerObjectGenerator::IOLT_DEFAULT);
}
void CSimpleEnemy::setPlayer(CPlayer *pPlayer) {
	dynamic_cast<CSimpleEnemyController*>(m_pCharacterController)->setPlayer(pPlayer);
}
