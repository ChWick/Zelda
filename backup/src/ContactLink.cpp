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
#include "ContactLink.h"
#include "ObjectManager.h"
#include "Player.h"

ContactLink::ContactLink(CMapManager &mapManager, CObjectManager &objectManager, const Ogre::SceneNode *pLinkNode, Ogre::Real radius, const UserData &userData, const Ogre::String &sID)
  : CObject(objectManager, CONTACT_LINK_OBJECT),
    CMapLinkInterface(mapManager,
                      MapLocalisationData(userData.getStringUserData("link_path"),
                                          userData.getStringUserData("link_name")),
                      userData.getStringUserData("link_id"),
                      sID,
                      CMapLinkInterface::parseCamPos(userData.getStringUserData("cam_pos"))),
    m_vPosition(pLinkNode->getPosition()),
    m_fRadius(radius) {

    m_PlayerPosition = m_vPosition;
    m_PlayerLookDirection = pLinkNode->getOrientation().zAxis();

    setUserData(userData);
    initLink(pLinkNode);
}

ContactLink::~ContactLink()
{
    //dtor
}
void ContactLink::update(Ogre::Real tpf) {
    if (m_ObjectManager.getPlayer()->getPosition().squaredDistance(m_vPosition) <= m_fRadius * m_fRadius) {
        if (m_NextMap.m_sName.empty()) {
            // not a valid link, this must be an entry point
            return;
        }
        activateLink();
    }
}
