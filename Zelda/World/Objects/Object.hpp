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
#include "InnerObjectGenerator.hpp"
#include "../WorldEntity.hpp"

class CObject : public CWorldEntity {
protected:
  const CObjectConstructionInfo &mSharedConstructionInfo;
  InnerObject::CGenerator mInnerObjectGenerator;
public:
   CObject(const std::string &id, CAbstractWorldEntity *pParent, CAbstractMap *pMap, EObjectTypes eObjectType, Ogre::SceneNode *pSceneNode = nullptr);
  ~CObject();

  virtual void init() override;
  virtual void exit() override;

  virtual void enterMap(CAbstractMap *pMap, const Ogre::Vector3 &vPosition) override;

  void setInnerObject(EObjectTypes eType);
  virtual void createInnerObject(EObjectTypes eType);

protected:
  void destroyPhysics();
  void createPhysics();

  void makePickable();

  virtual SInteractionResult interactOnCollision(const Ogre::Vector3 &vInteractDir, CAbstractWorldEntity *pSender) override;
  virtual SInteractionResult interactOnActivate(const Ogre::Vector3 &vInteractDir, CAbstractWorldEntity *pSender) override;
  virtual void changeState(EEntityStateTypes eState) override;
  virtual EReceiveDamageResult receiveDamage(const CDamage &dmg) override;
  virtual EReceiveDamageResult hit(const CDamage &dmg) override;



  virtual void killedCallback() override;
};

#endif // _OBJECT_HPP_
