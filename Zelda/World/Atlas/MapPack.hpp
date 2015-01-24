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

#ifndef _MAP_PACK_HPP_
#define _MAP_PACK_HPP_

#include <string>
#include <memory>
#include <OgreVector3.h>
#include <OgreVector2.h>
#include "../../Common/World/AbstractMapPack.hpp"

class CMapPack
    : public CAbstractMapPack {
private:
  std::string m_sSceneFile;

  Ogre::Vector3 m_vGlobalPosition;
  Ogre::Vector2 m_vGlobalSize;
  Ogre::Real    m_fVisionLevelOffset;

public:
  CMapPack(const std::string &path, const std::string &name);

  void init(CAbstractMapPackListener *pListener);
  void parse();

  const Ogre::Vector3 &getGlobalPosition() const {return m_vGlobalPosition;}
  const Ogre::Vector2 &getGlobalSize() const {return m_vGlobalSize;}
  const Ogre::Real &getVisionLevelOffset() const {return m_fVisionLevelOffset;}

  const std::string &getSceneFile() const {return m_sSceneFile;}

private:
  void parseXMLFile();
};

typedef std::shared_ptr<CMapPack> CMapPackPtr;
typedef std::shared_ptr<const CMapPack> CMapPackPtrConst;

#endif // _MAP_PACK_HPP_
