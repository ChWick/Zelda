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

#ifndef _ACTIONCREATEOBJECTCONSTRUCTIONINFO_HPP_
#define _ACTIONCREATEOBJECTCONSTRUCTIONINFO_HPP_

#include <string>
#include "ActionConstructionInfo.hpp"
#include "../../../../World/Objects/ObjectTypes.hpp"

namespace events {

class CActionCreateObjectConstructionInfo
    : public CActionConstructionInfo {
 private:
  EObjectTypes mObjectType;
  std::string mLocation;
 public:
  //! blank constructor
  CActionCreateObjectConstructionInfo();

  CActionCreateObjectConstructionInfo(const tinyxml2::XMLElement *e);

  EObjectTypes getObjectType() const {return mObjectType;}
  void setObjectType(const EObjectTypes ot) {mObjectType = ot;}

  const std::string &getLocation() const {return mLocation;}
  void setLocation(const std::string &l) {mLocation = l;}
};

};  // events

#endif /* _ACTIONCREATEOBJECTCONSTRUCTIONINFO_HPP_ */
