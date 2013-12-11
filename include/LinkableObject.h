#ifndef LINKABLEOBJECT_H
#define LINKABLEOBJECT_H

#include "Object.h"


class LinkableObject : public CObject {
    private:
        Ogre::SceneNode *m_pSceneNode;
        Ogre::Entity *m_pEntity;
    public:
        LinkableObject(CObjectManager &objectManager, btCollisionObject *pObject, Ogre::SceneNode *pSceneNode, Ogre::Entity *pEntity, const Ogre::String &id);
        virtual ~LinkableObject();
    protected:
    private:
};

#endif // LINKABLEOBJECT_H
