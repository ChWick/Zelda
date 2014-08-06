#ifndef _GAME_STATE_HPP_
#define _GAME_STATE_HPP_

#include "GameStateTypes.hpp"
#include "Entity.hpp"

class CGameState : public CEntity {
private:
public:
  CGameState(EGameStateTypes eType);
};

#endif // _GAME_STATE_HPP_
