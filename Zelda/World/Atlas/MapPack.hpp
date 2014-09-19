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
#include "../../Common/XMLResources/Manager.hpp"

class CMapPackParserListener;

class CMapPack {
private:
  const std::string m_sPath;
  const std::string m_sName;
  const std::string m_sResourceGroup;

  bool m_bInitialized;
  CMapPackParserListener *m_pListener;


  std::string m_sSceneFile;

  Ogre::Vector3 m_vGlobalPosition;
  Ogre::Vector2 m_vGlobalSize;

  XMLResources::CManager mLanguageManager;
public:
  CMapPack(const std::string &path, const std::string &name);
  ~CMapPack();

  void init(CMapPackParserListener *pListener);
  void parse();
  void exit();

  const std::string &getPath() const {return m_sPath;}
  const std::string &getName() const {return m_sName;}
  const std::string &getResourceGroup() const {return m_sResourceGroup;}

  const Ogre::Vector3 &getGlobalPosition() const {return m_vGlobalPosition;}
  const Ogre::Vector2 &getGlobalSize() const {return m_vGlobalSize;}

  const std::string &getSceneFile() const {return m_sSceneFile;}
  const XMLResources::CManager &getLanguageManager() const {return mLanguageManager;}

private:
  void parseXMLFile();
};

typedef std::shared_ptr<CMapPack> CMapPackPtr;
typedef std::shared_ptr<const CMapPack> CMapPackPtrConst;

#endif // _MAP_PACK_HPP_
