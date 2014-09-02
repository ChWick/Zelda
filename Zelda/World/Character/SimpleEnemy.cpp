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
#include <OgreEntity.h>
#include <OgreBone.h>
#include <OgreSkeletonInstance.h>

CSimpleEnemy::CSimpleEnemy(const std::string &sID, CEntity *pParent, EEnemyTypes eEnemyType)
	: CPerson(sID, pParent, PERSON_TYPE_ID_MAP.toData(PERSON_SOLDIER_BLUE), SE_ANIM_COUNT), m_eEnemyType(eEnemyType) {
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
  
	createHandObject(PERSON_RIGHT_HANDLE, RIGHT_HAND, "soldier_sword.mesh");
	createHandObject(PERSON_LEFT_HANDLE, LEFT_HAND, "soldier_shield.mesh");
}

void CSimpleEnemy::setupAnimations() {
  Ogre::StringVector animNames(SE_ANIM_COUNT);
	animNames[SE_ANIM_SCOUT] = "Scout";
	animNames[SE_ANIM_WALK] = "Walk";

	// this is very important due to the nature of the exported animations
  m_pBodyEntity->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);

  // populate our animation list
  for (unsigned int i = 0; i < m_uiAnimationCount; i++) {
    m_Anims[i] = m_pBodyEntity->getAnimationState(animNames[i]);
    m_Anims[i]->setLoop(true);
    m_FadingStates[i] = FADE_NONE;
    m_Anims[i]->setEnabled(false);
    m_Anims[i]->setWeight(0);
  }


  // start off in the idle state (top and bottom together)
  setAnimation(SE_ANIM_SCOUT);
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

void CSimpleEnemy::updateAnimationsCallback(const Ogre::Real fTime) {
}