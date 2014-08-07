#ifndef _CHARACTER_CONTROLLER_H_
#define _CHARACTER_CONTROLLER_H_

#include <list>
#include <OgreVector3.h>

class CharacterControllerListener;

class CCharacterController {
protected:
    std::list<CharacterControllerListener *> m_lListeners;
public:
    CCharacterController() {
    }
	virtual ~CCharacterController() {}

	void addListener(CharacterControllerListener *pListener) {m_lListeners.push_back(pListener);}
	void removeListener(CharacterControllerListener *pListener) {m_lListeners.remove(pListener);}

	virtual void updateCharacter(Ogre::Real tpf) = 0;

	virtual void setPosition(const Ogre::Vector3 &vPos) = 0;
  virtual void setOrientation(const Ogre::Quaternion &vRotation) = 0;

  virtual void moveToTarget(const Ogre::Vector3 &vPos, const Ogre::Real dRadius = 0.1f, const Ogre::Degree &maxDeviationLookDir = Ogre::Degree(5), bool bAddCharCOMHeight = true, const Ogre::Real fMaxDuration = 10.0f) = 0;
};
#endif
