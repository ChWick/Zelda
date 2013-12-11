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
