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

#ifndef _CHEST_HPP_
#define _CHEST_HPP_

#include "../WorldEntity.hpp"
#include "ObjectTypes.hpp"
#include "../../Common/PauseManager/PauseCaller.hpp"

class CChest : public CWorldEntity,
               public CPauseCaller {
public:
  enum EChestType {
    SMALL_CHEST,
    BIG_CHEST,
  };
private:
  enum EStatus {
    STATUS_CLOSED,
    STATUS_OPENING,
    STATUS_OPENED,
  };
  static const Ogre::Real INNER_OBJECT_LIFT_HEIGHT;
  static const Ogre::Real INNER_OBJECT_TIME_TO_LIFT;
  static const Ogre::Real INNER_OBJECT_TIME_TO_SET_IN_LIFT;

  Ogre::SceneNode *mLidSceneNode;
  Ogre::Vector3 mPhysicsOffset;
  const EChestType mChestType;
  EStatus mStatus;
  bool mLifting;
  CWorldEntity *mInnerObject;
  EObjectTypes mInnerObjectType;
  std::string mTextMessage;
  Ogre::Real mTimer;
public:
  CChest(const std::string &sID, CWorldEntity *pParent, CAbstractMap *pMap, EChestType chestType);
  ~CChest();

  void start();
  void update(Ogre::Real);
  void pauseUpdate(Ogre::Real);

  void setInnerObject(EObjectTypes eType) {mInnerObjectType = eType;}
  void createInnerObject(EObjectTypes eType);
private:

  SInteractionResult interactOnActivate(const Ogre::Vector3 &vInteractDir, CWorldEntity *pSender);
  void handleMessage(const CMessagePtr message);

  void open();
  void onLifted();
  void onFinished();
};

#endif // _CHEST_HPP_
