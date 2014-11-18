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
#include <string>
#include "SimpleEnemyController.hpp"
#include "../Atlas/Map.hpp"
#include "../Damage.hpp"
#include "PersonTypes.hpp"
#include <OgreEntity.h>
#include <OgreBone.h>
#include <OgreSkeletonInstance.h>
#include <OgreSceneNode.h>
#include "../../Common/Util/DebugDrawer.hpp"
#include "CharacterController_Physics.hpp"
#include "../Items/CharacterItem.hpp"

CSimpleEnemy::CSimpleEnemy(const std::string &sID,
                           CEntity *pParent,
                           CMap *pMap)
    : CPerson(sID,
              pParent,
              pMap,
              PERSON_DATA_ID_MAP.toData(PERSON_SOLDIER_GREEN_SWORD),
              SE_ANIM_COUNT) {
}

CSimpleEnemy::CSimpleEnemy(const tinyxml2::XMLElement *pElem,
                           CEntity *pParent,
                           CMap *pMap)
  : CPerson(pElem, pParent, pMap, SE_ANIM_COUNT) {
}
void CSimpleEnemy::setupInternal() {
  // createHandObject(PERSON_RIGHT_HANDLE, RIGHT_HAND, "soldier_sword.mesh");
  changeWeapon(PERSON_RIGHT_HANDLE, ITEM_VARIANT_ENEMY_SWORD_SIMPLE);
  // createHandObject(PERSON_LEFT_HANDLE, LEFT_HAND, "soldier_shield.mesh");

  getCurrentWeapon()->startDamage();
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

CSimpleEnemy::EReceiveDamageResult CSimpleEnemy::receiveDamage(
    const CDamage &dmg) {
  this->makeInvulnerable(1);
  dynamic_cast<CSimpleEnemyController*>(m_pCharacterController)
      ->changeMoveState(CSimpleEnemyController::MS_PUSHED_BACK,
                        dmg.getDamageDirection(), 0.2, 0.8);
  return RDR_ACCEPTED;
}

void CSimpleEnemy::killedCallback() {
  deleteLater();
  // InnerObjectGenerator::createInnerObject(*m_pCurrentMap,
  // getPosition(), InnerObjectGenerator::IOLT_DEFAULT);
}

void CSimpleEnemy::setPlayer(CWorldEntity *pPlayer) {
  dynamic_cast<CSimpleEnemyController*>(m_pCharacterController)
      ->setPlayer(pPlayer);
}

void CSimpleEnemy::updateAnimationsCallback(const Ogre::Real fTime) {
  for (CWorldEntity *pEnt
           : dynamic_cast<CharacterControllerPhysics*>(mCCPhysics)
           ->getCollidingWorldEntities()) {
    if (dynamic_cast<CCharacter*>(pEnt)
        && dynamic_cast<CCharacter*>(pEnt)->getFriendOrEnemyState()
        == FOE_FRIENDLY) {
      attack(CDamage(this,
                     DMG_SWORD,
                     m_pSceneNode->getOrientation().zAxis()), pEnt);
    }
  }

  CSimpleEnemyController::EKIState eKIState
      = dynamic_cast<CSimpleEnemyController*>(m_pCharacterController)
      ->getCurrentKIState();
  if (eKIState == CSimpleEnemyController::KI_PATROLING
      || eKIState == CSimpleEnemyController::KI_WALK_TO_PLAYER) {
    if (m_uiAnimID != SE_ANIM_WALK) {
      setAnimation(SE_ANIM_WALK, true);
    }
  } else if (eKIState == CSimpleEnemyController::KI_SCOUTING) {
    if (m_uiAnimID != SE_ANIM_SCOUT) {
      setAnimation(SE_ANIM_SCOUT, true);
    }
  }

  /*  const Ogre::Vector3 vDir = m_pBodyEntity->getParentNode()
      ->convertLocalToWorldOrientation(
          m_pBodyEntity->getSkeleton()->getBone(PERSON_RIGHT_HANDLE)
          ->_getDerivedOrientation()).yAxis();
  const Ogre::Vector3 vPos = m_pBodyEntity->getParentNode()
      ->convertLocalToWorldPosition(
          m_pBodyEntity->getSkeleton()->getBone(PERSON_RIGHT_HANDLE)
          ->_getDerivedPosition());

  DebugDrawer::getSingleton().drawLine(vPos,
                                       vPos + vDir * 0.1,
                                       Ogre::ColourValue::Blue);
  createDamage(Ogre::Ray(vPos, vDir * 0.1),
  CDamage(DMG_SWORD, m_pSceneNode->getOrientation().zAxis()));*/
  getCurrentWeapon()->updateDamage(fTime);
}
