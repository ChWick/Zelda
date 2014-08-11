#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Person.h"
#include <OgrePrerequisites.h>

class CMap;

using namespace Ogre;

class CPlayer : public CPerson {
private:
	const Ogre::Camera *m_pCamera;
	Ogre::SceneManager *m_pPlayerSceneManager;

	RibbonTrail* mSwordTrail;
public:

	CPlayer(CEntity *pParent, CMap *pMap, const Ogre::Camera *pCamera, Ogre::SceneManager *pPlayerSceneManager);
	virtual ~CPlayer();
	void destroy();
	void startup(const Ogre::Vector3 &playerPos, const Ogre::Vector3 &playerLookDirection, const Ogre::Real cameraYaw, const Ogre::Real cameraPitch);
protected:
	void setupInternal();
	void hitpointsChangedCallback();
	void maxHitpointsChangedCallback();
	void setupBody(SceneManager* sceneMgr);
	void setupCamera(Camera* cam);
	void preUpdateBoundsCallback(const Ogre::Real fTime);
	void updateWalkAnimation();
	void updateLiftedObject(const Ogre::Real fTime);

	virtual CCharacterController *createCharacterController();
};

#endif // _PLAYER_H_
