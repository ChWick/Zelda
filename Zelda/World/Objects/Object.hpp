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

#ifndef _OBJECT_HPP_
#define _OBJECT_HPP_

#include "ObjectTypes.hpp"
#include "../WorldEntity.hpp"

class CObject : public CWorldEntity {
protected:
public:
  CObject(const std::string &id, CWorldEntity *pParent, CMap *pMap, EObjectTypes eObjectType, Ogre::SceneNode *pSceneNode = nullptr);

protected:
  virtual SInteractionResult interactOnCollision(const Ogre::Vector3 &vInteractDir, CWorldEntity *pSender);
};

#endif // _OBJECT_HPP_
