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

#include "StdAfx.h"
#include "InteractionInterface.h"

InteractionInterface::EInteractDirection InteractionInterface::getInteractionDirectionFromString(const Ogre::String &id) {
    if (Ogre::StringUtil::match(id, "front", false)) {
        return ID_FRONT;
    }
    else if (Ogre::StringUtil::match(id, "right", false)) {
        return ID_RIGHT;
    }
    else if (Ogre::StringUtil::match(id, "rear", false)) {
        return ID_REAR;
    }
    else if (Ogre::StringUtil::match(id, "left", false)) {
        return ID_LEFT;
    }
    else if (Ogre::StringUtil::match(id, "all", false)) {
        return ID_ALL;
    }

    throw Ogre::Exception(__LINE__, id + " can not be converted to an interaction direction!", __FILE__);
}
Ogre::String InteractionInterface::getInteractionDirectionFromId(EInteractDirection id) {
    switch (id) {
    case ID_FRONT:
        return "front";
    case ID_REAR:
        return "rear";
    case ID_LEFT:
        return "left";
    case ID_RIGHT:
        return "right";
    }

    return "all";
}
const InteractionInterface::EInteractDirection InteractionInterface::getInteractDirection(const Ogre::Vector3 &vInteractDir, const Ogre::Real fYaw) {
    const Ogre::Real quaterPi(Ogre::Math::PI * 0.25);
    Ogre::Vector3 vTwoDDir(vInteractDir.x, 0, vInteractDir.z);
    //Ogre::Real radIntYaw(vTwoDDir.getRotationTo(Ogre::Vector3::UNIT_Z).getYaw().valueRadians());
    Ogre::Real radIntYaw(Ogre::Math::ATan2(vInteractDir.x, vInteractDir.z).valueRadians());
    Ogre::Real radInteractAngle(-radIntYaw + fYaw);
    if (radInteractAngle < -Ogre::Math::PI) {radInteractAngle += Ogre::Math::TWO_PI;}
    else if (radInteractAngle > Ogre::Math::PI) {radInteractAngle -= Ogre::Math::TWO_PI;}

    if (radInteractAngle > -quaterPi && radInteractAngle < quaterPi) {
        return ID_REAR;
    }
    else if (radInteractAngle > quaterPi && radInteractAngle < Ogre::Math::HALF_PI + quaterPi) {
        return ID_LEFT;
    }
    else if (radInteractAngle > Ogre::Math::HALF_PI + quaterPi || radInteractAngle < -Ogre::Math::HALF_PI - quaterPi) {
        return ID_FRONT;
    }
    else {
        return ID_RIGHT;
    }
}
