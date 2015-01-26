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

#ifndef ATTACHEDPARENTDATA_HPP
#define ATTACHEDPARENTDATA_HPP

#include <string>
#include "../Util/EnumIdMap.hpp"
#include "../tinyxml2/tinyxml2.hpp"

namespace DataContainers {

enum EAttachedParentTypes {
  APT_MAP,         //!< attached to the current map 
  APT_ATLAS,       //!< attached to the curent Atlas
  APT_WORLD,       //!< attached to the current World
  APT_LOCAL,       //!< attached to the local given node
  APT_ENTITY,      //!< attached to an given entity
};

class CAttachedParentData {
 private:
  //! Attachment type
  EAttachedParentTypes mType;
  //! If mType == APT_ENTITY this is the ID to attach to
  std::string mEntityID;

 public:
  CAttachedParentData();
  CAttachedParentData(const tinyxml2::XMLElement *e);

  EAttachedParentTypes getType() const {return mType;}
  const std::string &getEntityID() const {return mEntityID;}
};

class CAttachedParentTypesMap
    : public CStringEnumIdMap<CAttachedParentTypesMap,
                              EAttachedParentTypes> {
 public:
  void init() override;
};

}  // namespace DataContainers

#endif /* ATTACHEDPARENTDATA_HPP */
