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

#ifndef _REGION_HPP_
#define _REGION_HPP_

#include "../WorldEntity.hpp"
#include "RegionInfo.hpp"

class CRegion : public CWorldEntity {
  const SRegionInfo m_Info;
public:
  CRegion(CWorldEntity *pParent, const SRegionInfo &info);
  CRegion(CWorldEntity *pParent, const tinyxml2::XMLElement *pElem);
  ~CRegion();

  const Ogre::Vector3 &getPosition() const {return m_Info.position;}
  const Ogre::Vector3 &getSize() const {return m_Info.size;}


  void exit();

  void start();
};

#endif // _REGION_HPP_
