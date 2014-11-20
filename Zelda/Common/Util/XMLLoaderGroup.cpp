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

#include "XMLLoaderGroup.hpp"
#include <string>
#include <OgreException.h>
#include "../tinyxml2/tinyxml2.hpp"


using tinyxml2::XMLElement;

void CXMLLoaderGroup::registerGroup(const std::string &groupName,
                                        CXMLLoaderGroup *group) {
  mGroups[groupName] = group;
}

void CXMLLoaderGroup::readGroupElement(const tinyxml2::XMLElement *e) {
  for (const XMLElement *groupElement = e->FirstChildElement();
       groupElement;
       groupElement = groupElement->NextSiblingElement()) {
    auto it = mGroups.find(groupElement->Value());
    if (it == mGroups.end()) {
      throw Ogre::Exception(Ogre::Exception::ERR_INVALID_STATE,
                            "Unknown node '"
                            + std::string(groupElement->Value()) + "'",
                            __FILE__);
    }
    it->second->readGroupElement(groupElement);
  }
}
