#ifndef _SIMPLE_ENEMY_CONTROLLER_H_
#define _SIMPLE_ENEMY_CONTROLLER_H_

#include "PersonController.h"

class CPlayer;

class CSimpleEnemyController : public CPersonController {
private:
	CPlayer *m_pPlayer;			// the player, needed for KI calculations
public:
	CSimpleEnemyController(CPerson * ccPerson)
		: CPersonController(ccPerson), m_pPlayer(NULL) {
		changeMoveState(MS_USER_STATE);
	}
	void setPlayer(CPlayer *pPlayer) {assert(pPlayer); m_pPlayer = pPlayer;}
protected:
	void userUpdateCharacter(const Ogre::Real tpf);
	void postUpdateCharacter();
};

#endif