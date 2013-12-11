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
