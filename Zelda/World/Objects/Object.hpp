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
  const SObjectTypeData &m_ObjectTypeData;
  EObjectTypes mInnerObjectType;
public:
   CObject(const std::string &id, CWorldEntity *pParent, CMap *pMap, EObjectTypes eObjectType, Ogre::SceneNode *pSceneNode = nullptr);

  void init();

  virtual void enterMap(CMap *pMap, const Ogre::Vector3 &vPosition);

  void setInnerObject(EObjectTypes eType);
  virtual void createInnerObject(EObjectTypes eType);

protected:
  void destroyPhysics();
  void createPhysics();

  void makePickable();

  virtual SInteractionResult interactOnCollision(const Ogre::Vector3 &vInteractDir, CWorldEntity *pSender);
  virtual SInteractionResult interactOnActivate(const Ogre::Vector3 &vInteractDir, CWorldEntity *pSender);
  virtual void changeState(EEntityStateTypes eState);
  virtual EReceiveDamageResult receiveDamage(const CDamage &dmg);
  virtual EReceiveDamageResult hit(const CDamage &dmg);



  virtual void killedCallback();
};

#endif // _OBJECT_HPP_
