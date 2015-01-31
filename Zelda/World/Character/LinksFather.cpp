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

#include "LinksFather.hpp"
#include <OgreEntity.h>
#include <OgreBone.h>
#include <OgreSkeletonInstance.h>
#include <OgreSceneNode.h>
#include "PersonController.hpp"

CLinksFather::CLinksFather(CAbstractWorldEntity *pParent,
                           const CPersonConstructionInfo &info)
    : CPerson(pParent, info, LF_ANIM_COUNT) {
}

void CLinksFather::updateAnimationsCallback(const Ogre::Real fTime) {
  uint8_t ms = getCharacterController<CPersonController>()->getMoveState();
  if (ms == CPersonController::MS_MOVE_TO_POINT) {
    setAnimation(LF_ANIM_WALK);
  }
}

CCharacterController *CLinksFather::createCharacterController() {
  CPersonController *pc = dynamic_cast<CPersonController*>(
      CPerson::createCharacterController());
  pc->setWalkSpeed(3);
  return pc;
}
