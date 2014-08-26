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

#ifndef CONTACTLINK_H
#define CONTACTLINK_H

#include "Object.h"
#include "MapLinkInterface.h"

class ContactLink : public CObject, public CMapLinkInterface {
private:
    const Ogre::Real m_fRadius;
    const Ogre::Vector3 m_vPosition;
public:
    ContactLink(CMapManager &mapManager, CObjectManager &objectManager, const Ogre::SceneNode *pLinkNode, Ogre::Real radius, const UserData &userData, const Ogre::String &sID);
    virtual ~ContactLink();

    void update(Ogre::Real tpf);
protected:
private:
};

#endif // CONTACTLINK_H
