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

#ifndef ABSTRACTMAPPACK_HPP
#define ABSTRACTMAPPACK_HPP

#include "../XMLResources/Manager.hpp"

class CAbstractMapPackListener;

class CAbstractMapPack {
 protected:
  const std::string mPath;
  const std::string mName;
  const std::string mResourceGroup;

  bool mInitialized;
  
  XMLResources::CManager mLanguageManager;

  CAbstractMapPackListener *mListener;
 public:
  CAbstractMapPack(const std::string &path,
                   const std::string &name);
  virtual ~CAbstractMapPack();

  virtual void init(CAbstractMapPackListener *listener);
  virtual void exit();

  virtual void parse() = 0;

  const std::string &getPath() const {return mPath;}
  const std::string &getName() const {return mName;}
  const std::string &getResourceGroup() const {return mResourceGroup;}
  
  const XMLResources::CManager &getLanguageManager() const {return mLanguageManager;}
};


typedef std::shared_ptr<CAbstractMapPack> CAbstractMapPackPtr;
typedef std::shared_ptr<const CAbstractMapPack> CAbstractMapPackPtrConst;

#endif /* ABSTRACTMAPPACK_HPP */
