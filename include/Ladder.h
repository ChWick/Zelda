#ifndef LADDER_H
#define LADDER_H

#include <Object.h>


class Ladder : public CObject {
private:
    const Ogre::Real m_fYawRadians; //!< the rotation of the ladder, to know whether a climber is in front/back of the ladder, to interact
    Ogre::Real m_fLength;     //!< the length of the ladder
    Ogre::Vector3 m_vLadderDir;
    Ogre::Vector3 m_vClimberPositionBot;
public:
    Ladder(CObjectManager &objectManager, const Ogre::Vector3 &vPos, const Ogre::Real fYawRadians);
    virtual ~Ladder();

	virtual void update(Ogre::Real tpf);

    Ogre::Vector3 getClimberPosition(const Ogre::Real fPos) const;

    bool atBottom(const Ogre::Real fPos) const {return fPos < 0;}
    bool atTop(const Ogre::Real fPos) const {return fPos > m_fLength;}

    const Ogre::Real getYawRadians() const  {return m_fYawRadians;}
    Ogre::Vector3 getDirection() const {return Ogre::Vector3(Ogre::Math::Sin(m_fYawRadians), 0, Ogre::Math::Cos(m_fYawRadians));}
    Ogre::Vector3 getTopExitPos() const;
    Ogre::Vector3 getBottomExitPos() const;

	virtual bool interactOnCollision(const btCollisionObject *pPartner);
protected:
private:
};

#endif // LADDER_H
