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

#ifndef WORLDENTITY_HPP
#define WORLDENTITY_HPP

#include "../Common/World/AbstractWorldEntity.hpp"

class CWorldEntity
    : public CAbstractWorldEntity {
 public:
  CWorldEntity(const std::string &sID, CEntity *pParent, CAbstractMap *pMap, const std::string &sResourceGroup = Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
  CWorldEntity(CEntity *pParent, CAbstractMap *pMap, const tinyxml2::XMLElement *pElem, const std::string &sResourceGroup = Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
  CWorldEntity(CAbstractWorldEntity *parent, const CWorldEntityConstructionInfo &info);

 public:
  static CWorldEntity *getFromUserPointer(btCollisionObject *pCO) {
    return static_cast<CWorldEntity*>(CAbstractWorldEntity::getFromUserPointer(pCO));
  }
  static CWorldEntity *getFromUserPointer(const btCollisionObject *pCO) {
    return static_cast<CWorldEntity*>(CAbstractWorldEntity::getFromUserPointer(pCO));
  }
};

#endif /* WORLDENTITY_HPP */
