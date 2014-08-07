#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Person.h"

class CMap;
class CCameraController;

using namespace Ogre;

class CPlayer : public CPerson {
private:
	Quaternion m_qPickedObjectInitialRotationToPlayer;
	bool m_bFreeCamera;
	bool m_bShiftPressed;
	bool m_bMoveToInteractivePoint;
	bool m_bUserMayMove;

	//const CLadder *m_pLadder;						//!< Player auf der Leiter?
	Ogre::Real m_fLadderPos;						//!< Position auf der Leiter [0,1]
	Ogre::Real m_fLadderStep;						//!< In weclhe richtung soll ein Schritt gemacht werden {-1, 0, 1}
	bool m_bGoingToLadder;							//!< Animation zur leiter, oder von der Leiter zu normal

	Ogre::SceneManager *m_pPlayerSceneManager;
	CCameraController *m_pCameraController;

	RibbonTrail* mSwordTrail;

	Ogre::Light *m_pLight;                          //!< Possible light of the player

public:

	CPlayer(CMap *pMap, CCameraController* camController, Ogre::SceneManager *pPlayerSceneManager);
	virtual ~CPlayer();
	void destroy();
	void startup(const Ogre::Vector3 &playerPos, const Ogre::Vector3 &playerLookDirection, const Ogre::Real cameraYaw, const Ogre::Real cameraPitch);

	void setPlayerMayMove(bool b) {m_bUserMayMove = b;}

	void interact(CObject::EInteractType eInteractType);

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

	virtual bool hasMoreArrows();
    virtual void removeArrow();
    virtual bool hasMoreBombs();
    virtual void removeBomb();
};

#endif // _PLAYER_H_
