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

#ifndef _SIMPLE_ENEMY_H_
#define _SIMPLE_ENEMY_H_

#include "Person.hpp"

class CPlayer;

class CSimpleEnemy : public CPerson {
 public:
  enum ESimpleEnemyAnimations {
    SE_ANIM_SCOUT,
    SE_ANIM_WALK,

    SE_ANIM_COUNT,
  };

 private:
 public:
  CSimpleEnemy(CAbstractWorldEntity *pParent,
               const CPersonConstructionInfo &info);

  void setPlayer(CWorldEntity *pPlayer);

 protected:
  void setupInternal();
  void setupAnimations();
  virtual CCharacterController *createCharacterController();
  EReceiveDamageResult receiveDamage(const CDamage &dmg);
  void killedCallback();

  void updateAnimationsCallback(const Ogre::Real fTime);
};

#endif
