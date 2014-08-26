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

#ifndef _TOOL_H_
#define _TOOL_H_

#include "StdAfx.h"
#include "PlayerTool.h"

class CTool {
protected:
    Ogre::Entity *m_pEntity;
    const unsigned int m_uiToolId;
    Ogre::SceneManager *m_pSceneManager;
public:
    CTool(unsigned int uiToolId, Ogre::SceneManager *pSM)
        : m_pEntity(NULL), m_uiToolId(uiToolId), m_pSceneManager(pSM) {
        createMesh();
    }
    virtual ~CTool() {
        m_pSceneManager->destroyEntity(m_pEntity);
        m_pEntity = NULL;
    }
    virtual void start() {}
    virtual void update (Ogre::Real tpf) {
    }
    Ogre::Entity *getEntity() const {return m_pEntity;}
    unsigned int getId() const {return m_uiToolId;}

protected:
    virtual void createMesh() {
        Ogre::String meshFile = CPlayerTool::MESH_FILES[m_uiToolId];
        m_pEntity = m_pSceneManager->createEntity(meshFile);
    }
};

#endif
