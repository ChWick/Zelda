#ifndef _SIMPLE_ENEMY_H_
#define _SIMPLE_ENEMY_H_

#include "Person.h"

class CPlayer;

class CSimpleEnemy : public CPerson {
public:
	enum EEnemyTypes {
		ET_GREEN_SWORD,
		ET_BLOCKER,         // soldier that stands still and only blocks the player (start of game e.g.)
	};
private:
	const EEnemyTypes m_eEnemyType;
public:
	CSimpleEnemy(const std::string &sID, CEntity *pParent, EEnemyTypes eEnemyType);

	void setPlayer(CPlayer *pPlayer);

protected:
	void setupInternal();
	virtual CCharacterController *createCharacterController();
	void killedCallback();
};

#endif
