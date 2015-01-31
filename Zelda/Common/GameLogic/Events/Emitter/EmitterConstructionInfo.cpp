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

#include "EmitterConstructionInfo.hpp"
#include <OgreException.h>
#include "EmitOnReceivedDamageConstructionInfo.hpp"
#include "EmitOnInteractionConstructionInfo.hpp"
#include "../../../tinyxml2/tinyxml2.hpp"
#include "../../../Util/XMLHelper.hpp"

using XMLHelper::Attribute;

namespace events {

CEmitterConstructionInfo::CEmitterConstructionInfo(const EEmitterTypes t)
    : mType(t) {
}

std::shared_ptr<CEmitterConstructionInfo> CEmitterConstructionInfo::createNew(
    const tinyxml2::XMLElement *e) {
  const EEmitterTypes t = CEmitterTypesMap::getSingleton().parseString(
      Attribute(e, "type"));

  switch (t) {
    case EMIT_ON_CREATE:
      return std::make_shared<CEmitterConstructionInfo>(t);
    case EMIT_ON_RECEIVED_DAMAGE:
      return std::make_shared<CEmitOnReceivedDamageConstructionInfo>(e);
    case EMIT_ON_INTERACTION:
      return std::make_shared<CEmitOnInteractionConstructionInfo>(e);
    default:
      throw Ogre::Exception(t, "Emitter type not implemented!",
                            __FILE__);
  }
}

}  // namespace events
