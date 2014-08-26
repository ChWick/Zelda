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
#include "Ladder.h"
#include "Person.h"
#include "DebugDrawer.h"

Ladder::Ladder(CObjectManager &objectManager, const Ogre::Vector3 &vPos, const Ogre::Real fYawRadians)
 : CObject(objectManager, LADDER_OBJECT), m_fYawRadians(fYawRadians) {
    //ctor
    m_fLength = 2.55;
    m_vClimberPositionBot = vPos + Ogre::Vector3(Ogre::Math::Sin(fYawRadians), 0, Ogre::Math::Cos(fYawRadians)) * 0.1f + Ogre::Vector3::UNIT_Y * 0.5f;
    Ogre::Real fPitch = Ogre::Degree(15).valueRadians();
    m_vLadderDir = Ogre::Vector3(Ogre::Math::Sin(fPitch) * Ogre::Math::Sin(fYawRadians), Ogre::Math::Cos(fPitch), -Ogre::Math::Sin(fPitch) * Ogre::Math::Cos(fYawRadians));
}
Ladder::~Ladder() {
    //dtor
}
void Ladder::update(Ogre::Real tpf) {
    DebugDrawer::getSingleton().drawLine(getClimberPosition(0), getClimberPosition(m_fLength), Ogre::ColourValue::Red);

};
bool Ladder::interactOnCollision(const btCollisionObject *pPartner) {
    CPhysicsUserPointer *pPhysUserPointer = static_cast<CPhysicsUserPointer*>(pPartner->getUserPointer());
    CPerson *pPerson = dynamic_cast<CPerson*>(pPhysUserPointer);
    if (!pPerson) {return false;}

    auto interactionDir = getInteractDirection(pPerson->getBodyNode()->getOrientation().zAxis(), m_fYawRadians);
    if (interactionDir == ID_REAR || interactionDir == ID_FRONT) {
        if (!pPartner->getUserPointer()) {return false;}
        // has to be a person to be able interact
        // now tell the person to go on the ladder

        // if the person comes from the rear, he has to be at the top of the ladder!
        if (interactionDir == ID_REAR && pPerson->getPosition().y < getClimberPosition(m_fLength * 0.9).y) {return false;}

        pPerson->climbLadder(this, (interactionDir == ID_REAR) ? m_fLength : 0);

        return true;
    }
    return false;
}
Ogre::Vector3 Ladder::getClimberPosition(const Ogre::Real fPos) const {
    assert(fPos >= 0 && fPos <= m_fLength);
    return m_vLadderDir * fPos + m_vClimberPositionBot;
}
Ogre::Vector3 Ladder::getTopExitPos() const {
    return getClimberPosition(m_fLength) - getDirection() * 1;
}
Ogre::Vector3 Ladder::getBottomExitPos() const {
    return getClimberPosition(0) + getDirection() * 1;
}
