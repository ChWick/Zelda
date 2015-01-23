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

#include "EventConstructionInfo.hpp"
#include <OgreException.h>
#include "Actions/ActionConstructionInfo.hpp"
#include "../../tinyxml2/tinyxml2.hpp"
#include "../../Util/XMLHelper.hpp"

using tinyxml2::XMLElement;

using XMLHelper::Attribute;
using XMLHelper::RealAttribute;
using XMLHelper::BoolAttribute;

namespace events {

CEventConstructionInfo::CEventConstructionInfo()
    : mID(""),
      mRepeatType(REPEAT_NONE),
      mRepeatTime(0),
      mTimer(0),
      mStarted(false) {
}

CEventConstructionInfo::CEventConstructionInfo(const XMLElement *e)
    : mID(Attribute(e, "id", "")),
      mRepeatType(CRepeatTypesMap::getSingleton().parseString(
          Attribute(e, "repeat_type",
                    CRepeatTypesMap::getSingleton().toString(REPEAT_NONE)))),
      mRepeatTime(RealAttribute(e, "repeat_time", 0)),
      mTimer(RealAttribute(e, "timer", 0)),
      mStarted(BoolAttribute(e, "started", false)) {
  for (const XMLElement *c = e->FirstChildElement(); c;
       c = c->NextSiblingElement()) {
    if (strcmp(c->Value(), "action") == 0) {
      mActionConstructionInfos.push_back(
          CActionConstructionInfo::createNew(c));
    } else if (strcmp(c->Value(), "emitter") == 0) {
      mEmitterConstructionInfos.push_back(
          CEmitterConstructionInfo::createNew(c));
    } else {
      throw Ogre::Exception(0,
                            "'" + Ogre::String(c->Value()) + "' is not a valid "
                            "child tag of an event construction info.",
                            __FILE__);
    }
  }
}

}  // namespace events
