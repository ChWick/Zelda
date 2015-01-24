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

#ifndef _INTERACTION_INTERFACE_HPP_
#define _INTERACTION_INTERFACE_HPP_

#include <OgrePrerequisites.h>

class CAbstractWorldEntity;

class CInteractionInterface {
public:
  enum EInteractionDirection {
    ID_NONE  = 0,
    ID_FRONT = 1,
    ID_REAR  = 2,
    ID_LEFT  = 4,
    ID_RIGHT = 8,
    ID_ALL   = ID_FRONT | ID_REAR | ID_LEFT | ID_RIGHT,
  };

  enum EInteractionResult {
    IR_NONE,
    IR_LIFT,
  };

  struct SInteractionResult {
    const EInteractionResult eResult;

    SInteractionResult(EInteractionResult r = IR_NONE)
     : eResult(r) {
    }
    SInteractionResult(SInteractionResult &&) = default;
  };

public:
  virtual SInteractionResult interactOnCollision(const Ogre::Vector3 &vInteractDir, CAbstractWorldEntity *pSender) {
    return SInteractionResult();
  }
  virtual SInteractionResult interactOnActivate(const Ogre::Vector3 &vInteractDir, CAbstractWorldEntity *pSender) {
    return SInteractionResult();
  }
};

#endif // _INTERACTION_INTERFACE_HPP_
