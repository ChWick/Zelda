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

#include "EntityConstructionInfo.hpp"
#include <string>
#include "../Util/XMLHelper.hpp"
#include <OgreException.h>
#include <OgreResourceGroupManager.h>

using XMLHelper::Attribute;
using XMLHelper::IntAttribute;
using XMLHelper::BoolAttribute;

using tinyxml2::XMLElement;

CEntityConstructionInfo::CEntityConstructionInfo(const std::string &id)
    : mID(id),
      mResourceGroup(
          Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME),
      mType(0),
      mState(EST_NORMAL),
      mPauseRender(false),
      mPauseUpdate(false) {
}

CEntityConstructionInfo::CEntityConstructionInfo(const XMLElement *e)
    : mID(Attribute(e, "id", "SharedConstructionInfo")),
      mResourceGroup(Attribute(e, "resource_group",
                               Ogre::ResourceGroupManager
                               ::AUTODETECT_RESOURCE_GROUP_NAME)),
      mType(IntAttribute(e, "type", 0)),
      mState(CEntityStateIdMap::getSingleton().parseString(
          Attribute(e, "state",
                    CEntityStateIdMap::getSingleton().toString(
                        EST_NORMAL)))),
      mPauseRender(BoolAttribute(e, "pause_render", false)),
      mPauseUpdate(BoolAttribute(e, "pause_update", false)) {
  for (const XMLElement *c = e->FirstChildElement(); c;
       c = c->NextSiblingElement()) {
    if (strcmp(c->Value(), "event") == 0) {
      mEventConstructionInfos.push_back(
          std::make_shared<events::CEventConstructionInfo>(c));
    } else {
      throw Ogre::Exception(0,
                            "'" + Ogre::String(c->Value()) + "' is not a valid"
                            " child tag of an entity construction information",
                            __FILE__);
    }
  }
}
