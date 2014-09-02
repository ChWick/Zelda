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

#ifndef _CHARACTER_CONTROLLER_H_
#define _CHARACTER_CONTROLLER_H_

#include <list>
#include <OgreVector3.h>

class CCharacterController {
protected:
public:
    CCharacterController() {
    }
	virtual ~CCharacterController() {}

  virtual inline void start() {}

	virtual void updateCharacter(Ogre::Real tpf) = 0;

	virtual void setPosition(const Ogre::Vector3 &vPos) = 0;
  virtual void setOrientation(const Ogre::Quaternion &vRotation) = 0;

  virtual void moveToTarget(const Ogre::Vector3 &vPos, const Ogre::Real dRadius = 0.1f, const Ogre::Degree &maxDeviationLookDir = Ogre::Degree(5), bool bAddCharCOMHeight = true, const Ogre::Real fMaxDuration = 10.0f) = 0;
};
#endif
