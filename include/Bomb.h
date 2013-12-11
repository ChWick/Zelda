#ifndef BOMB_H
#define BOMB_H

#include "LiftableObject.h"

class CBombListener;

class CBomb : public CLiftableObject {
private:
    Ogre::SceneNode *m_pSceneNode;
    Ogre::Real m_fTimer;
    CBombListener *m_pListener;
public:
    CBomb(Ogre::SceneNode *pParentNode, CObjectManager &objectManager, const Ogre::Vector3 &vPosition);
    virtual ~CBomb();

    void setListener(CBombListener *pListener) {m_pListener = pListener;}

	virtual void update(Ogre::Real tpf);
protected:
private:
    void explode();
};

#endif // BOMB_H
