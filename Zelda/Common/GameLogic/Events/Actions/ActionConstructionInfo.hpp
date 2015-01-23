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

#ifndef _ACTION_CONSTRUCTION_INFO_HPP_
#define _ACTION_CONSTRUCTION_INFO_HPP_

#include <memory>
#include "ActionTypes.hpp"

namespace tinyxml2 {
class XMLElement;
};

namespace events {

class CActionConstructionInfo {
 private:
  const EActionTypes mType;

 protected:
  //! protected construction since this is not a valid class
  CActionConstructionInfo(const EActionTypes t);

 public:
  const EActionTypes &getType() const {return mType;}

  static std::shared_ptr<CActionConstructionInfo>
  createNew(const tinyxml2::XMLElement *e);
};

};  // events

#endif /* _ACTION_CONSTRUCTION_INFO_HPP_ */
