#ifndef GLOWER_H
#define GLOWER_H

#include <Object.h>


class Glower : public CObject {
public:
    static const Ogre::Real DEFAULT_BURNING_TIME;
private:
    Ogre::ParticleSystem *m_pFireParticleSystem;
    Ogre::SceneNode *m_pFireSceneNode;
    Ogre::Real m_fTimer;
    const Ogre::Real m_fBurningTime;
public:
    Glower(CObjectManager &objectManager, Ogre::SceneNode *pSceneNode, const Ogre::Real fBurningTime = DEFAULT_BURNING_TIME);
    virtual ~Glower();

	virtual void update(Ogre::Real tpf);

	EReceiveDamageResult receiveDamage(const CDamage &damage);
protected:
private:
};

#endif // GLOWER_H
