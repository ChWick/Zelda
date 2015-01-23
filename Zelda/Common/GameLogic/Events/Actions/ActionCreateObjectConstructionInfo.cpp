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

#include "ActionCreateObjectConstructionInfo.hpp"
#include "../../../Util/XMLHelper.hpp"

using tinyxml2::XMLElement;
using XMLHelper::Attribute;

namespace events {

CActionCreateObjectConstructionInfo::CActionCreateObjectConstructionInfo()
    : CActionConstructionInfo(ACTION_CREATE_OBJECT),
      mObjectType(OBJECT_COUNT) {
}

CActionCreateObjectConstructionInfo::CActionCreateObjectConstructionInfo(
    const tinyxml2::XMLElement *e)
    : CActionConstructionInfo(ACTION_CREATE_OBJECT),
      mObjectType(CObjectTypeIdMap::getSingleton().parseString(
          Attribute(e, "object_type"))),
      mLocation(Attribute(e, "location")) {
}

}  // namespace events
