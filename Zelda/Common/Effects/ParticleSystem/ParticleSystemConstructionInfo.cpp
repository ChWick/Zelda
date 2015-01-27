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

#include "ParticleSystemConstructionInfo.hpp"
#include <string>
#include "../../Util/XMLHelper.hpp"

using tinyxml2::XMLElement;

using XMLHelper::Attribute;
using XMLHelper::RealAttribute;
using XMLHelper::Vector3Attribute;

CParticleSystemConstructionInfo::CParticleSystemConstructionInfo()
    : mType("unknown"),
      mScale(Ogre::Vector3::UNIT_SCALE),
      mScaleVelocity(1),
      mOffset(Ogre::Vector3::ZERO) {
}

CParticleSystemConstructionInfo::CParticleSystemConstructionInfo(
    const tinyxml2::XMLElement *e)
    : mType(Attribute(e, "type")),
      mScale(Vector3Attribute(e, "scale", Ogre::Vector3::UNIT_SCALE)),
      mScaleVelocity(RealAttribute(e, "scale_velocity", 1)),
      mOffset(Vector3Attribute(e, "offset", Ogre::Vector3::ZERO)) {
}
